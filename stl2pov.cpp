#include <array>
#include <iostream>
#include <fstream>
#include <limits>

#include <map>
#include <set>
#include <utility>

using namespace std;

int main(int argc, char * argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " input.stl > output.pov" << endl;
        return 0;
    }

    ifstream in(argv[1], ios_base::in | ios_base::binary);

    // read 80-char header - leave a byte for null term
    char header[81] = {};
    in.read(header, 80);
    cout << "// " << header << endl << endl;

    // get original triangle count
    unsigned char buffer[4];
    in.read((char *)buffer, 4);
    unsigned int triangle_count = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
    cout << "// Triangle count: " << triangle_count << endl << endl;

    // read triangles to end
    map<array<unsigned char, 12>, unsigned int> v_map;
    set<array<array<unsigned char, 12>, 3>> t_set;

    for (unsigned int i = 0; i < triangle_count; i ++)
    {
        // vector of normal - ignore
        in.seekg(12, ios_base::cur);

        // three pts
        array<array<unsigned char, 12>, 3> t;

        for (unsigned j = 0; j < 3; j ++)
        {
            //array<unsigned char, 12> v;
            in.read((char *)t[j].data(), 12);
            v_map.insert(make_pair(t[j], 0));

            //t[j] = v;
        }

        // extra
        in.seekg(2, ios_base::cur);

        // insert triangle into set
        // this should attempt to re-order the vertices so that
        //  the smallest value is first
        // can help with de-duplication
        if (t[0] < t[1])
            if (t[0] < t[2])
                t_set.insert( { t[0], t[1], t[2] } );
            else
                t_set.insert( { t[2], t[0], t[1] } );
        else if (t[1] < t[2])
            t_set.insert( { t[1], t[2], t[0] } );
        else
            t_set.insert( { t[2], t[0], t[1] } );
    }

    in.close();

    string filename = argv[1];
    // Remove extension if present.
    const size_t period_idx = filename.rfind('.');
    if (string::npos != period_idx)
    {
        filename.erase(period_idx);
    }

    cout << "#declare stl_" << filename << " = mesh2 {" << endl;
    cout << "  vertex_vectors {" << endl << "    " << v_map.size() << endl;

    // sets up output formatting for full precision available from float
    cout.precision(numeric_limits<float>::max_digits10 - 1);

    cout << scientific;

    // iterate the v-map and write it out
    unsigned int i = 0;
    for (auto v = v_map.begin(); v != v_map.end(); v ++)
    {
        cout << "    <";
        for (unsigned int j = 0; j < 3; j ++)
        {
            float f = * ((float*)(v->first.data() + 4 * j));
            cout << f << ( j == 2 ? ">\n" : ", ");
        }

        v->second = i;
        i ++;
    }

    // now the triangles
    cout << "  }" << endl;
    cout << fixed;
    cout << "  face_indices {" << endl << "    " << t_set.size() << endl;
    for (auto t = t_set.begin(); t != t_set.end(); t ++)
    {
        cout << "    <";
        for (unsigned int j = 0; j < 3; j ++)
        {
            cout << v_map.at(t->at(j)) << ( j == 2 ? ">\n" : ", ");
        }
    }
    cout << "  }" << endl;
    cout << "}" << endl;

    return 0;
}
