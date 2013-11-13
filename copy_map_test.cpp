#include <iostream>
// mapnik
#include <mapnik/load_map.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
// boost
#include <boost/timer/timer.hpp>

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
        }
    }

    {
        std::cerr << "parsing xml style " << NUM_RUNS << " times.. " << std::endl;
        boost::timer::auto_cpu_timer t;
        for (int i=0; i< NUM_RUNS; ++i)
        {
            mapnik::load_map(m, argv[1]);
        }
    }

    std::cerr << "Done" << std::endl;
    return 0;
}
