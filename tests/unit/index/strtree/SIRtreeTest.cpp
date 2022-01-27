#include <tut/tut.hpp>
// geos
#include <geos/index/strtree/SIRtree.h>
#include <geos/index/quadtree/Quadtree.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>
#include <geos/index/strtree/SimpleSTRtree.h>
#include <geos/index/strtree/GeometryItemDistance.h>
#include <geos/index/ItemVisitor.h>
#include <geos/io/WKTReader.h>

using namespace geos::index::strtree;

namespace tut {
// dummy data, not used
struct test_sirtree_data {};

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
()
{
    SIRtree t;
    double value = 3;
    t.insert(1, 5, &value);
}
template<>
template<> void object::test<2>
()
{
    geos::index::quadtree::Quadtree quadtree;
    std::vector<std::unique_ptr<geos::geom::Geometry>> geoms;
    const int gridSize = 10;

    auto gf = geos::geom::GeometryFactory::create();
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            geos::geom::Coordinate c((double)i, (double)j);
            geos::geom::Coordinate c1((double)i +5, (double)j+5);
            geos::geom::Point* pt = gf->createPoint(c);
            geos::geom::Point* pt1 = gf->createPoint(c1);
            geos::geom::Polygon* poly = gf->createPolygon();
            geoms.emplace_back(pt);
            quadtree.insert(pt->getEnvelopeInternal(),pt);
        }
    }

    geos::geom::Envelope qe(-0.5, 1.5, -0.5, 1.5);
    std::vector<void*> matches;
    std::cout << "quadtree" <<matches.size() << std::endl;
}


} // namespace tut

