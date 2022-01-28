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

using namespace geos::index::strtree;

namespace tut {
// dummy data, not used
    struct test_sirtree_data {
    };

    using group = test_group<test_sirtree_data>;
    using object = group::object;

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
        geos::index::quadtree::Quadtree quadtree;
        std::vector<geos::geom::Geometry> geoms;
        std::vector<geos::geom::Geometry *> poly_vec;
        std::vector<geos::geom::Geometry *> poly_vec_qw;
        std::unique_ptr<geos::geom::PrecisionModel> pm(new geos::geom::PrecisionModel());
        geos::geom::GeometryFactory::Ptr global_factory = geos::geom::GeometryFactory::create(pm.get(), -1);
        const int num_of_poly = 10;
        for (auto i = 0; i < num_of_poly; i++) {
            double length = (double) rand() / (RAND_MAX + 1.0);
            double minX = -i;
            double minY = i;
            double maxX = -(i + length + (rand() % 5));
            double maxY = i + length + (rand() % 5);
            geos::geom::CoordinateArraySequence *cl6 = new geos::geom::CoordinateArraySequence();
            cl6->add(geos::geom::Coordinate(minX + 1.11123, minY + 1.23223));
            cl6->add(geos::geom::Coordinate(minX + 1.561587, maxY + 5.451355));
            cl6->add(geos::geom::Coordinate(minX + 4.126513, maxY + 3.526651));
            cl6->add(geos::geom::Coordinate(minX + 2.268415, maxY + 6.465681));
            cl6->add(geos::geom::Coordinate(maxX + 1.23456, maxY + 2.89462));
            cl6->add(geos::geom::Coordinate(maxX + 1.65156, minY + 5.54655));
            cl6->add(geos::geom::Coordinate(maxX + 1.21465, minY + 1.123544));
            cl6->add(geos::geom::Coordinate(minX + 1.11123, minY + 1.23223));
            geos::geom::LinearRing *lr6 = global_factory->createLinearRing(cl6);
            geos::geom::Polygon *poly6 = global_factory->createPolygon(lr6, NULL);
            poly_vec.push_back(poly6);
            quadtree.insert(poly6->getEnvelopeInternal(), poly6);
        }
        std::size_t  size = quadtree.index_size();
        std::cout << "quadtree size is" <<  size << " " << std::endl;

    }

/*

    geos::geom::Envelope qe(-0.5, 1.5, -0.5, 1.5);
    std::vector<void*> matches;
    std::cout << "quadtree" <<matches.size() << std::endl;
}
*/

} // namespace tut
