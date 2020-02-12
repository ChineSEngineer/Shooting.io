#include <gameWorld/maps.hpp>
#include <iostream>

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <json/json.hpp>


class World {
 public:
    using json = nlohmann::json;

    World() {
        Map1 map;
        obstacles_ = std::move(map.blocks);
        // for (auto it = obstacles_.begin() ; it != obstacles_.end(); ++it) {
        //     std::cout << "???" << std::endl;
        //     std::cout << (*it)->line1.end1().x << " " << (*it)->line1.end1().y << "     "
        //     << (*it)->line1.end2().x << " " << (*it)->line1.end2().y << std::endl;
        // }
        // obstacles_.insert( std::make_shared<Line>( vec(1,1), vec(3, 1) ));
        // obstacles_.insert( std::make_shared<Line>( vec(1,1), vec(1, 3) ));
    }


    std::string get_game_state() {
        json j, j_players = json::array(), j_bullets = json::array(), j_walls = json::array();
        for (auto it = players_.begin(); it != players_.end(); ++it) {
            json tmp;
            tmp["x"] = it->second->position().x;
            tmp["y"] = it->second->position().y;
            j_players.push_back(tmp);
        }

        for (auto&& b : bullets_) {
            json tmp;
            tmp["x"] = b->position().x;
            tmp["y"] = b->position().y;
            j_bullets.push_back(tmp);
        }

        for (auto&& block : obstacles_) {
            json tmp;
            tmp["x"] = block->position().x;
            tmp["y"] = block->position().y;
            j_walls.push_back(tmp);
        }
        j["players"] = j_players;
        j["bullets"] = j_bullets;
        j["walls"] = j_walls;
        return j.dump();
    }

    void parse_user_control(const std::string& jstr, const std::string& ip) {
        //try {
            bool input_a, input_w, input_d, input_s, input_space;
            json j = json::parse(jstr);
            auto findw = j.find("w");
            auto finda = j.find("a");
            auto finds = j.find("s");
            auto findd = j.find("d");
            auto findspace = j.find(" ");
            if (findw != j.end() && finda != j.end() &&
                finds != j.end() && findd != j.end()) {
                input_w = findw.value();
                input_a = finda.value();
                input_s = finds.value();
                input_d = findd.value();
                input_space = findspace.value();
            }
            int x_speed = 0;
            int y_speed = 0;
            if (input_w) {
                y_speed -= 1;
            }
            if (input_s) {
                y_speed += 1;
            }
            if (input_d) {
                x_speed += 1;
            }
            if (input_a) {
                x_speed -= 1;
            }
            if (input_space) {
                user_shoot(ip);
            }
            user_control(x_speed, y_speed, ip);
        //} catch (std::exception& e) {i

        //}
    }

    void add_player(const std::string& ip) {
        if (players_.find(ip) == players_.end()) {
            players_[ip] = std::make_shared<Person>(world_size_);
        }
    }

    void update(double time) {
        update_items(time);
        check_hit();
        clean();
    }


 private:
    void user_control(int x, int y, const std::string& ip) {
        if (players_.find(ip) == players_.end()) {
            // players_[ip] = std::make_shared<Person>(world_size_);
            std::cout << "------------------ Cannot find player" << std::endl;
            return;
        }
        players_[ip]->set_speed({(double) x, (double) y});
    }

    void user_shoot(const std::string& ip) {
        if (players_.find(ip) == players_.end()) {
            // players_[ip] = std::make_shared<Person>(world_size_);
            return;
        }
        std::shared_ptr<Person> owner = players_[ip];
        bullets_.insert(std::make_shared<Bullet>(owner->position(), owner->towards() ,owner));
    }

    void clean() {
#ifndef NDEBUG
        std::cout << __FUNCTION__ << std::endl;
#endif
        for (auto it = bullets_.begin(); it != bullets_.end();) {
            if ((*it)->destroyed()) {
                it = bullets_.erase(it);
            } else {
                ++it;
            }
        }
        for (auto it = players_.begin(); it != players_.end();) {
            if (it->second->destroyed()) {
                it = players_.erase(it);
            } else {
                ++it;
            }
        }
#ifndef NDEBUG
        std::cout << __FUNCTION__ << "end" << std::endl;
#endif
    }

    void check_hit() {
        for (auto&& bullet : bullets_) {
            for (auto&& p : players_) {
                auto& player = p.second;
                if (bullet->owner() == player) {
                    continue;
                }
                if (player->distance(*bullet) < player->size()) {
                    bullet->set_destroyed(true);
                    player->injure(bullet->damage());
                }
            }
        }
    }

    void update_items(double time) {
#ifndef NDEBUG
        std::cout << __FUNCTION__ << std::endl;
#endif
        for (auto&& bullet : bullets_) {
            bool update = true;
            for (auto&& obstacle : obstacles_) {
                if (obstacle->distance(*bullet) < obstacle->size()) {
                    bullet->set_destroyed(true);
                    update = false;
                }
            }
            if (update) {
                bullet->update(time);
            }
        }
        for (auto it = players_.begin(); it != players_.end(); ++it) {
            bool update = true;
            auto& player = it->second;
            for (auto&& obstacle : obstacles_) {
                if (obstacle->collision(*player, time)) {
                    update = false;
                }
            }
            if (update) {
                it->second->update(time);
            }
        }
#ifndef NDEBUG
        std::cout << __FUNCTION__ << "end" << std::endl;
#endif
    }

 private:
    std::unordered_map<std::string, std::shared_ptr<Person>> players_;
    // std::unordered_set<std::shared_ptr<Block>> obstacles_;
    std::unordered_set<std::shared_ptr<Block>> obstacles_;
    std::unordered_set<std::shared_ptr<Bullet>> bullets_;
    double world_size_ = 20;
};
