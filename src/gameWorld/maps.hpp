#ifndef MAPS_HPP
#define MAPS_HPP

#include "physical_object.hpp"

#include <memory>
#include <unordered_set>

class Map1 {
 public:

    Map1() {
        blocks.insert(  std::make_shared<Block>(vec(0,0)));
        blocks.insert(  std::make_shared<Block>(vec(0,1)));
        blocks.insert(  std::make_shared<Block>(vec(0,2)));
        blocks.insert(  std::make_shared<Block>(vec(0,3)));
        blocks.insert(  std::make_shared<Block>(vec(0,4)));
        blocks.insert(  std::make_shared<Block>(vec(0,5)));
        blocks.insert(  std::make_shared<Block>(vec(0,6)));
        blocks.insert(  std::make_shared<Block>(vec(0,7)));
        blocks.insert(  std::make_shared<Block>(vec(0,8)));
        blocks.insert(  std::make_shared<Block>(vec(0,9)));
        blocks.insert(  std::make_shared<Block>(vec(0,10)));
        blocks.insert(  std::make_shared<Block>(vec(0,11)));
        blocks.insert(  std::make_shared<Block>(vec(0,12)));
        blocks.insert(  std::make_shared<Block>(vec(0,13)));
        blocks.insert(  std::make_shared<Block>(vec(0,14)));
        blocks.insert(  std::make_shared<Block>(vec(0,15)));
        blocks.insert(  std::make_shared<Block>(vec(0,16)));
        blocks.insert(  std::make_shared<Block>(vec(0,17)));
        blocks.insert(  std::make_shared<Block>(vec(0,18)));
        blocks.insert(  std::make_shared<Block>(vec(0,19)));

        blocks.insert(  std::make_shared<Block>(vec(19,0)));
        blocks.insert(  std::make_shared<Block>(vec(19,1)));
        blocks.insert(  std::make_shared<Block>(vec(19,2)));
        blocks.insert(  std::make_shared<Block>(vec(19,3)));
        blocks.insert(  std::make_shared<Block>(vec(19,4)));
        blocks.insert(  std::make_shared<Block>(vec(19,5)));
        blocks.insert(  std::make_shared<Block>(vec(19,6)));
        blocks.insert(  std::make_shared<Block>(vec(19,7)));
        blocks.insert(  std::make_shared<Block>(vec(19,8)));
        blocks.insert(  std::make_shared<Block>(vec(19,9)));
        blocks.insert(  std::make_shared<Block>(vec(19,10)));
        blocks.insert(  std::make_shared<Block>(vec(19,11)));
        blocks.insert(  std::make_shared<Block>(vec(19,12)));
        blocks.insert(  std::make_shared<Block>(vec(19,13)));
        blocks.insert(  std::make_shared<Block>(vec(19,14)));
        blocks.insert(  std::make_shared<Block>(vec(19,15)));
        blocks.insert(  std::make_shared<Block>(vec(19,16)));
        blocks.insert(  std::make_shared<Block>(vec(19,17)));
        blocks.insert(  std::make_shared<Block>(vec(19,18)));
        blocks.insert(  std::make_shared<Block>(vec(19,19)));

        blocks.insert(  std::make_shared<Block>(vec(1,0)));
        blocks.insert(  std::make_shared<Block>(vec(2,0)));
        blocks.insert(  std::make_shared<Block>(vec(3,0)));
        blocks.insert(  std::make_shared<Block>(vec(4,0)));
        blocks.insert(  std::make_shared<Block>(vec(5,0)));
        blocks.insert(  std::make_shared<Block>(vec(6,0)));
        blocks.insert(  std::make_shared<Block>(vec(7,0)));
        blocks.insert(  std::make_shared<Block>(vec(8,0)));
        blocks.insert(  std::make_shared<Block>(vec(9,0)));
        blocks.insert(  std::make_shared<Block>(vec(10,0)));
        blocks.insert(  std::make_shared<Block>(vec(11,0)));
        blocks.insert(  std::make_shared<Block>(vec(12,0)));
        blocks.insert(  std::make_shared<Block>(vec(13,0)));
        blocks.insert(  std::make_shared<Block>(vec(14,0)));
        blocks.insert(  std::make_shared<Block>(vec(15,0)));
        blocks.insert(  std::make_shared<Block>(vec(16,0)));
        blocks.insert(  std::make_shared<Block>(vec(17,0)));
        blocks.insert(  std::make_shared<Block>(vec(18,0)));

        blocks.insert(  std::make_shared<Block>(vec(1,19)));
        blocks.insert(  std::make_shared<Block>(vec(2,19)));
        blocks.insert(  std::make_shared<Block>(vec(3,19)));
        blocks.insert(  std::make_shared<Block>(vec(4,19)));
        blocks.insert(  std::make_shared<Block>(vec(5,19)));
        blocks.insert(  std::make_shared<Block>(vec(6,19)));
        blocks.insert(  std::make_shared<Block>(vec(7,19)));
        blocks.insert(  std::make_shared<Block>(vec(8,19)));
        blocks.insert(  std::make_shared<Block>(vec(9,19)));
        blocks.insert(  std::make_shared<Block>(vec(10,19)));
        blocks.insert(  std::make_shared<Block>(vec(11,19)));
        blocks.insert(  std::make_shared<Block>(vec(12,19)));
        blocks.insert(  std::make_shared<Block>(vec(13,19)));
        blocks.insert(  std::make_shared<Block>(vec(14,19)));
        blocks.insert(  std::make_shared<Block>(vec(15,19)));
        blocks.insert(  std::make_shared<Block>(vec(16,19)));
        blocks.insert(  std::make_shared<Block>(vec(17,19)));
        blocks.insert(  std::make_shared<Block>(vec(18,19)));

        blocks.insert(  std::make_shared<Block>(vec(5,5) ));
        blocks.insert(  std::make_shared<Block>(vec(7,9) ));
    }

 public:
    std::unordered_set<std::shared_ptr<Block>> blocks;
};


#endif  // MAPS_HPP
