#include <iostream>
// mapnik
#include <mapnik/expression.hpp>
#include <mapnik/expression_grammar.hpp>
#include <mapnik/expression_string.hpp>
#include <mapnik/expression_evaluator.hpp>
#include <mapnik/config_error.hpp>

// boost
#include <boost/timer/timer.hpp>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage:" << argv[0] << " <expression> <num-runs>" << std::endl;
        return 1;
    }

    const int NUM_RUNS = std::stol(argv[2]);

    std::cerr << "Parsing expression ..." << std::endl;

    mapnik::expression_ptr expr;
    try
    {
        expr = mapnik::parse_expression(argv[1], "utf8");
        std::string str = mapnik::to_expression_string(*expr);
        std::cerr << str << std::endl;
    }
    catch (mapnik::config_error & ex)
    {
        std::cerr << ex.what() << std::endl;
    }


    {
        std::cerr << "copying expr " << NUM_RUNS << " times.. " << std::endl;
        boost::timer::auto_cpu_timer t;
        for (int i=0; i< NUM_RUNS; ++i)
        {
            mapnik::expression_ptr copy_node = std::make_shared<mapnik::expr_node>(*expr);
            if (i == 0)
            {
                std::cerr << mapnik::to_expression_string(*copy_node) << std::endl;
            }
        }
    }

    {
        std::cerr << "parsing expr " << NUM_RUNS << " times.. " << std::endl;
        boost::timer::auto_cpu_timer t;
        for (int i=0; i< NUM_RUNS; ++i)
        {
            std::string str = mapnik::to_expression_string(*expr);
            mapnik::expression_ptr node = mapnik::parse_expression(str, "utf8");
            if (i == 0)
            {
                std::cerr << mapnik::to_expression_string(*node) << std::endl;
            }
        }
    }


    {
        std::cerr << "parsing expr (reusing grammar)" << NUM_RUNS << " times.. " << std::endl;
        boost::timer::auto_cpu_timer t;
        mapnik::transcoder tr("utf8");
        mapnik::expression_grammar<std::string::const_iterator> g(tr);
        for (int i=0; i< NUM_RUNS; ++i)
        {
            std::string str = mapnik::to_expression_string(*expr);
            mapnik::expression_ptr node = mapnik::parse_expression(str, g);
            if (i == 0)
            {
                std::cerr << mapnik::to_expression_string(*node) << std::endl;
            }
        }
    }


    std::cerr << "Done" << std::endl;
    return 0;
}
