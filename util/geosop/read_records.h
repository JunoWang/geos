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
            if (filename.find("csv") != std::string::npos or filename.find("wkt") != std::string::npos) {
                assert(!tokens[0].empty());
                assert(tokens[0] != " ");
                // remove the double quote marks in WKT polygon (WKT form does not have double quotes)
                tokens[0].erase(std::remove(tokens[0].begin(), tokens[0].end(), '\"'), tokens[0].end());
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
            } else if (filename.find("park") != std::string::npos) {
                assert(!tokens[1].empty());
                assert(tokens[1] != " ");
                tokens[1].erase(std::remove(tokens[1].begin(), tokens[1].end(), '\"'), tokens[1].end());
                try {
                    std::unique_ptr<geos::geom::Geometry> geom_a(wkt_reader.read(tokens[1]));
                    geom_vector.push_back(global_factory->createGeometry(geom_a.get()));
//            std::cout << geom_a.get()->toString() << std::endl;
                } catch (geos::io::ParseException exception) {
                    skipped++;
                    continue;
                }
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
