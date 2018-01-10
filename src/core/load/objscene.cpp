
#include "load/loader.h"
#include "math/vec.h"
#include <fstream> 
#include <iostream> 
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <unordered_map>

using namespace bpm;

static inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

struct vert{
    int vi = 0;
    int vti = 0;
    int vni = 0;
    void set(std::string fstr)
    {
        std::vector<std::string> elements = split(fstr, '/');
        //do fall through
        switch( elements.size() ){
            case 3: vni = atoi(elements[2].c_str()) - 1; 
            case 2: vti = ("" == elements[1])? 0 : atoi(elements[1].c_str()) - 1; 
            case 1: vi = atoi(elements[0].c_str()) - 1; 
        }
    }
    bool operator==(const vert &other) const
    { 
        return (vi == other.vi && vti == other.vti && vni == other.vni);
    }
};

namespace std
{
    template <>
    struct hash<vert>
    {
        size_t operator()( const vert& v ) const
        {
            // Compute individual hash values for each member var
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash<int>()( v.vi );
            res = res * 31 + hash<int>()( v.vti );
            res = res * 31 + hash<int>()( v.vni );
            return res;
        }
    };
}

struct triangle{
    vert a,b,c;
    triangle(std::string v0, std::string v1, std::string v2){
        a.set(v0);
        b.set(v1);
        c.set(v2);
    }
};

struct file_data {
    vector<vec3> v;
    vector<vec3> vc;
    vector<vec2> vt;
    vector<vec3> vn;
};

static void process_vert(const vert& v, vector<int>& indices, 
                         vector<float>& positions, const vector<vec3>& pos,
                         vector<float>& texcoords, const vector<vec2>& vt,
                         vector<float>& normals,   const vector<vec3>& vn, 
                         std::unordered_map<vert,int>& vert_index_map, int& vcount )
{
    //if this vertex has already been processed (or one identical to it), 
    //then add its existing index position.
    int vindex = vert_index_map[v] - 1;
    if( vindex >= 0 ){ 
        indices.push_back(vindex); 
        return;
    }

    //this is a new unique vertex, so we must add a new indexed entry
    //and track that index
    vindex = vcount++;
    vert_index_map[v] = vcount; // = vindex+1
    
    indices.push_back(vindex);

    vec3 p = pos[v.vi];
    positions.push_back(p.x);
    positions.push_back(p.y);
    positions.push_back(p.z);

    vec2 t = vt[v.vti];
    texcoords.push_back(t.x);
    texcoords.push_back(1-t.y);

    vec3 n = vn[v.vni];
    normals.push_back(n.x);
    normals.push_back(n.y);
    normals.push_back(n.z);
}


static shared_ptr<Model> model_from_obj_stream(MeshLoader& loader, std::ifstream& in, std::unordered_map<std::string, std::shared_ptr<Material>>& material_map, file_data& fdata){

        vector<vec3>& v = fdata.v;
        vector<vec3>& vc = fdata.vc;
        vector<vec2>& vt = fdata.vt;
        vector<vec3>& vn = fdata.vn;
        vector<triangle> tris;
        std::shared_ptr<Material> material;

        std::string line;
        while (std::getline(in,line)) 
        {
            std::string type = line.substr(0,2);
            if (type == "v ") 
            {
                std::istringstream s(line.substr(2));
                vec3 vertex(0,0,0); 
                s >> vertex.x; 
                s >> vertex.y; 
                s >> vertex.z;
                v.push_back(vertex);
                
                vec3 color(0,0,0); 
                if( s.peek() != EOF ) 
                {
                    s >> color.x; 
                    s >> color.y; 
                    s >> color.z;
                }
                vc.push_back(color);
            }
            else if (type == "vn")
            {
                std::istringstream s(line.substr(2));
                vec3 normal(0,0,0); 
                s >> normal.x; 
                s >> normal.y; 
                s >> normal.z;
                vn.push_back(normal.normalize());
            }
           else if (type == "vt")
            {
                std::istringstream s(line.substr(2));
                vec2 tex(0,0); 
                s >> tex.x; 
                s >> tex.y; 
                vt.push_back(tex);
            }
            else if (type == "f ")
            {
                std::istringstream s(line.substr(2));
                std::string a,b,c;
                s >> a; 
                s >> b; 
                s >> c;
                tris.emplace_back(a,b,c);
            }
            else if (type == "us")//usemtl
            {
                std::string mat_name = line.substr(7);
                material = material_map[mat_name];
            }
            
            //don't start reading from the next object!
            if( in.peek() == 'o' )  {
                break;
            }
            //ignore anything else
        }

        if(vt.size() == 0){
            vt.emplace_back(0,0);
        }
        if(vn.size() == 0){
            vn.emplace_back(0,0,1);
        }

        vector<float> positions;
        vector<float> texcoords;
        vector<float> normals;
        vector<int> indices;

        std::unordered_map<vert,int> vert_index_map;
        int vcount = 0;
        
        for( triangle t : tris ){
            process_vert(t.a, indices, positions, v, texcoords, vt, normals, vn, vert_index_map, vcount);
            process_vert(t.b, indices, positions, v, texcoords, vt, normals, vn, vert_index_map, vcount);
            process_vert(t.c, indices, positions, v, texcoords, vt, normals, vn, vert_index_map, vcount);
        }

        auto mesh = loader.load(positions, texcoords, normals, indices);
        return std::make_shared<Model>(mesh, material);
    }

    static std::shared_ptr<Material> material_from_mtl_stream(std::ifstream& in){

        vec3 diffuse_color(1,1,1);
        vec3 specular_color(1,1,1);
        float specular_exp = 50;
        float transparency = 1;

        std::string line;
        while (std::getline(in,line)) 
        {
            // end of material def
            if( line.length() == 0 ){
                break;
            }

            std::string type = line.substr(0,2);
            if (type == "Ns"){
                specular_exp = std::stof(line.substr(3));
                continue;
            }

            if (type == "Kd"){
                std::istringstream s(line.substr(3));
                s >> diffuse_color.x; 
                s >> diffuse_color.y; 
                s >> diffuse_color.z;
                continue;
            }

            if (type == "Ks"){
                std::istringstream s(line.substr(3));
                s >> specular_color.x; 
                s >> specular_color.y; 
                s >> specular_color.z;
                continue;
            }

            if (type == "d "){
                // dissolved == transparency
                transparency = std::stof(line.substr(2));
                continue;
            }
        }
        auto material = std::make_shared<BasicMaterial>(diffuse_color, specular_color, specular_exp, transparency);
        return material;
    }

    static void load_materials(std::unordered_map<std::string, std::shared_ptr<Material>>& material_map, std::string& mat_file){
        std::ifstream in(mat_file, std::ios::in);
        if(!in){
            std::cout << "fatal error: could not load obj material file: " << mat_file << std::endl;
            return;
        }
        std::string line;
        while (std::getline(in,line)) 
        {
            std::string type = line.substr(0,6);
            if (type == "newmtl"){
                std::string mat_name = line.substr(7);
                auto mat = material_from_mtl_stream(in);
                material_map[mat_name] = mat;
                
                continue;
            }
        }
    }


namespace Load {
    vector<shared_ptr<Model>> obj_model(MeshLoader& loader, const std::string& obj_file){

        std::ifstream in(obj_file, std::ios::in);
        if(!in){
            std::cout << "fatal error: could not load obj file: " << obj_file << std::endl;
            exit(1);
        } 

        file_data fdata;
        vector<shared_ptr<Model>> model_list;
        std::unordered_map<std::string, std::shared_ptr<Material>> materials;

        std::string line;
        while (std::getline(in,line)) 
        {
            std::string type = line.substr(0,2);
            if (type == "o "){
                std::string obj_name = line.substr(2);
                model_list.push_back(model_from_obj_stream(loader, in, materials, fdata));
                continue;
            }

            if (type == "mt"){
                std::string mat_file = line.substr(7);
                load_materials(materials, mat_file);
            }
        }
        return model_list;
    }
}