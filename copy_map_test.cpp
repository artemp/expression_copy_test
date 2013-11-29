#include <iostream>
// mapnik
#include <mapnik/load_map.hpp>
#include <mapnik/save_map.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/value.hpp>
#include <mapnik/expression_node_types.hpp>
#include <mapnik/expression_string.hpp>
#include <mapnik/symbolizer.hpp>
#include <mapnik/enumeration.hpp>
#include <mapnik/image_compositing.hpp>

// boost
#include <boost/timer/timer.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/at.hpp>

#include <cassert>

int main(int argc, char** argv)
{
    const static std::string MAPNIK_DIR="/opt/mapnik";

    if (argc != 3)
    {
        std::cerr << "Usage:" << argv[0] << " <map-xml> <num-runs>" << std::endl;
        return 1;
    }

    const int NUM_RUNS = std::stol(argv[2]);
    mapnik::datasource_cache::instance().register_datasources(MAPNIK_DIR+"/lib/mapnik/input");
    mapnik::freetype_engine::register_fonts(MAPNIK_DIR+"/lib/mapnik/fonts");
    std::cerr << "Parsing XML stylesheet..." << std::endl;

    mapnik::Map m(256,256);

    try
    {
        mapnik::load_map(m, argv[1]);
    }
    catch (mapnik::config_error & ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    {
        std::cerr << "copying map " << NUM_RUNS << " times.. " << std::endl;
        boost::timer::auto_cpu_timer t;
        for (int i=0; i< NUM_RUNS; ++i)
        {
            mapnik::Map new_map(m); // copy ctor
            if (i == 0)
            {
                if (mapnik::save_map_to_string(m) != mapnik::save_map_to_string(new_map))
                {
                    throw std::runtime_error("FAIL");
                }
            }
        }
    }

    {
        std::cerr << "parsing xml style " << NUM_RUNS << " times.. " << std::endl;
        boost::timer::auto_cpu_timer t;
        for (int i=0; i< NUM_RUNS; ++i)
        {
            mapnik::Map new_map(256,256);
            mapnik::load_map(new_map, argv[1]);
        }
    }

    std::cerr << "Done" << std::endl;

    return 0;
}
