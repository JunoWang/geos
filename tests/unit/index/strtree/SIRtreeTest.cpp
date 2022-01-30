#include <tut/tut.hpp>
// geos
#include <geos/index/strtree/SIRtree.h>
#include <geos/index/quadtree/Quadtree.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/Dimension.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Point.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/io/WKTReader.h>
#include <geos/util/IllegalArgumentException.h>
#include <utility.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace geos::index::strtree;

namespace tut {
// dummy data, not used
    struct test_sirtree_data {
    };

    using group = test_group<test_sirtree_data>;
    using object = group::object;
    void read_whole_records(std::string filename, std::vector<geos::geom::Geometry *> &geom_vector) {

        std::unique_ptr<geos::geom::PrecisionModel> pm(new geos::geom::PrecisionModel());
        geos::geom::GeometryFactory::Ptr global_factory = geos::geom::GeometryFactory::create(pm.get(), -1);
        geos::io::WKTReader wkt_reader(*global_factory);

        int skipped = 0;
        std::ifstream infile(filename);
        // read a whole line/row
        std::string line;
        while (std::getline(infile, line)) {
            std::istringstream iss{line}; // construct a string stream from line
            // line_part:  value of each column of one row
            // tokens[i] is the value of row[i]
            if (!line.empty()) {
                std::string line_part;
                std::vector<std::string> tokens;
                while (std::getline(iss, line_part, '\t')) {
                    tokens.push_back(line_part); // add the line_part to the vector
                }
                assert(!tokens[0].empty());
                assert(tokens[0] != " ");
                // remove the double quote marks in WKT polygon (WKT form does not have double quotes)
                tokens[0].erase(std::remove(tokens[0].begin(), tokens[0].end(), '\"'), tokens[0].end());
//        std::cout << " the polygon WKT from dataset is "<<tokens[0] << std::endl;
//        Create geos WKT reader and read into geometry vector
//        Need use get to get the geometry
                try {
                    std::unique_ptr<geos::geom::Geometry> geom_a(wkt_reader.read(tokens[0]));
                    geom_vector.push_back(global_factory->createGeometry(geom_a.get()));
//            std::cout << geom_a.get()->toString() << std::endl;
                } catch (geos::io::ParseException exception) {
                    skipped++;
                    continue;
                }
            } else {
                skipped++;
                continue;
            }

        }
        assert(!geom_vector.empty());
        std::cout << filename << " has " << geom_vector.size() << " records" << std::endl;
        std::cout << filename << " skips " << skipped << " records" << std::endl;
    }

    group test_sirtree_group("geos::index::strtree::SIRtree");

//
// Test Cases
//

// Make sure no memory is leaked.
// See https://trac.osgeo.org/geos/ticket/919
    template<>
    template<>
    void object::test<1>
            () {
        SIRtree t;
        double value = 3;
        t.insert(1, 5, &value);
    }

    template<>
    template<>
    void object::test<2>
            () {
//        geos::index::quadtree::Quadtree quadtree;
//        std::vector<geos::geom::Geometry> geoms;
//        std::vector<geos::geom::Geometry *> poly_vec;
//        std::vector<geos::geom::Geometry *> poly_vec_qw;
//        std::unique_ptr<geos::geom::PrecisionModel> pm(new geos::geom::PrecisionModel());
//        geos::geom::GeometryFactory::Ptr global_factory = geos::geom::GeometryFactory::create(pm.get(), -1);
//        const int num_of_poly = 10;
//        for (auto i = 0; i < num_of_poly; i++) {
//            double length = (double) rand() / (RAND_MAX + 1.0);
//            double minX = -i;
//            double minY = i;
//            double maxX = -(i + length + (rand() % 5));
//            double maxY = i + length + (rand() % 5);
//            geos::geom::CoordinateArraySequence *cl6 = new geos::geom::CoordinateArraySequence();
//            cl6->add(geos::geom::Coordinate(minX + 1.11123, minY + 1.23223));
//            cl6->add(geos::geom::Coordinate(minX + 1.561587, maxY + 5.451355));
//            cl6->add(geos::geom::Coordinate(minX + 4.126513, maxY + 3.526651));
//            cl6->add(geos::geom::Coordinate(minX + 2.268415, maxY + 6.465681));
//            cl6->add(geos::geom::Coordinate(maxX + 1.23456, maxY + 2.89462));
//            cl6->add(geos::geom::Coordinate(maxX + 1.65156, minY + 5.54655));
//            cl6->add(geos::geom::Coordinate(maxX + 1.21465, minY + 1.123544));
//            cl6->add(geos::geom::Coordinate(minX + 1.11123, minY + 1.23223));
//            geos::geom::LinearRing *lr6 = global_factory->createLinearRing(cl6);
//            geos::geom::Polygon *poly6 = global_factory->createPolygon(lr6, NULL);
//            poly_vec.push_back(poly6);
//            quadtree.insert(poly6->getEnvelopeInternal(), poly6);
//        }
//        std::size_t  size = quadtree.index_size();
//        std::cout << "quadtree size is" <<  size << " " << std::endl;

    }
    template<>
    template<>
    void object::test<3>
            () {
        std::string path = "/Users/juno/Desktop/glin_dataset/TIGER_2015_AREAWATER.csv";
        std::vector<geos::geom::Geometry*> geoms;
        read_whole_records(path,geoms);
        geos::index::quadtree::Quadtree quadtree1;
        for (size_t i = 0; i < geoms.size(); i++) {
            quadtree1.insert(geoms[i]->getEnvelopeInternal(), geoms[i]);
        }
        std::size_t  size = quadtree1.index_size();
        std::cout << "quadtree with " << " " << path << size << std::endl;
    }



/*

    geos::geom::Envelope qe(-0.5, 1.5, -0.5, 1.5);
    std::vector<void*> matches;
    std::cout << "quadtree" <<matches.size() << std::endl;
}
*/

} // namespace tut
