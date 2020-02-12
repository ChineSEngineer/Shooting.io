#ifndef PHYSICAL_OBJECTS_HPP
#define PHYSICAL_OBJECTS_HPP

#include <iostream>
#include <memory>
#include <initializer_list>
#include <limits>
#include <cmath>

struct vec {
    vec& operator=(const vec& other) {
        x = other.x;
        y = other.y;
    }

    double distance(const vec& other) {
        double xd = x - other.x;
        double yd = y - other.y;
        return std::sqrt(xd * xd + yd * yd);
    }
    vec(const vec& other) : x{other.x}, y{other.y}{} vec() : x{0}, y{0} {} vec(double a, double b) : x(a), y(b) {} double x;
    double y;
};

class PhysicalObject {
 public:
    PhysicalObject (const vec& position, const vec& speed)
        : position_{position},
          speed_{speed},
          destroyed_{false},
          speed_length_{0} {}

    vec position() const { return position_; }
    vec speed() const { return speed_; }
    void set_speed(const vec& speed) {
        double tmp_x = speed.x;
        double tmp_y = speed.y;
        int len = std::sqrt(tmp_x * tmp_x + tmp_y * tmp_y);
        if (len != 0) {
            speed_ = {tmp_x / len, tmp_y / len};
        } else {
            speed_ = {0, 0};
        }
    }
    bool destroyed() const { return destroyed_; }
    void set_destroyed(bool value) { destroyed_ = value; }
    double distance(const PhysicalObject& other) {
        return (this->position_).distance(other.position_);
    }
    void update(double time) {
        position_.x += time * speed_.x * speed_length_;
        position_.y += time * speed_.y * speed_length_;
    }
    vec potential_position(double time) const {
        double x = position_.x + time * speed_.x * speed_length_;
        double y = position_.y + time * speed_.y * speed_length_;
        return {x, y};
    }
 protected:
    vec position_;
    vec speed_;
    bool destroyed_;
    double speed_length_;
};


class Person : public PhysicalObject {
 public:
    Person(double position_x, double position_y, double speed_x, double speed_y)
        : PhysicalObject({position_x, position_y}, {speed_x, speed_y}) {
        speed_length_ = person_speed_length;
    }

    Person (double world_size) : PhysicalObject({0, 0}, {0, 0}) {
        srand (time(NULL));
        position_.x = (double)rand() / RAND_MAX * world_size;
        position_.y = (double)rand() / RAND_MAX * world_size;
        speed_length_ = person_speed_length;
    }

    vec towards() const {
        return towards_; 
    }

    void set_speed(const vec& speed) {
        PhysicalObject::set_speed(speed);
        if (speed_.x != 0 || speed_.y != 0) {
            towards_ = speed_;
        }
    }

    bool operator==(const Person& other) const {
        return this == &other;
    }
    double size() const {
        return size_;
    }
    double injure(int damage) {
        health_ -= damage;
        if (health_ < 0) {
            destroyed_ = true;
        }
    }
 private:
    double size_ = 0.2;
    int health_ = 10;
    double person_speed_length = 0.006;
    vec towards_ = {0.0, 1.0};;
};

// TODO: 子弹飞行边界
class Bullet : public PhysicalObject {
 public:
    Bullet(const vec& position, const vec& speed, const std::shared_ptr<Person> owner)
        : PhysicalObject({position.x, position.y}, {speed.x, speed.y}),
          owner_{owner} {
        speed_length_ = bullet_speed_length;
    }
    // Bullet(double position_x, double position_y,
    //        double speed_x, double speed_y,
    //        const std::shared_ptr<Person> owner)
    //     : PhysicalObject({position_x, position_y}, {speed_x, speed_y}),
    //       owner_{owner} {
    //     speed_length_ = bullet_speed_length;
    // }
    const std::shared_ptr<Person>& owner() const {
        return owner_;
    }
    int damage() {
        return damage_;
    }
 private:
    const std::shared_ptr<Person> owner_;
    int damage_ = 1;
    double bullet_speed_length = 0.012;
};



class Line : public PhysicalObject {
 public:
    Line(const vec& end1, const vec& end2) : PhysicalObject(
                                                    {(end1.x + end2.x)/2, (end1.y + end2.y)/2},
                                                    {0,0} ) {
        end1_ = end1;
        end2_ = end2;
        slope_ = slope(end1_, end2_);
        intercept_ = intercept(end1_, slope_);
    }

    vec end1() const {
        return end1_;
    }

    vec end2() const {
        return end2_;
    }

    void set_end1(const vec& v) {
        end1_ = v;
    }

    void set_end2(const vec& v) {
        end2_ = v;
    }

    bool collision(const PhysicalObject& bullet, double time) {
        vec potential = bullet.potential_position(time);
        if (bullet.position().x == potential.x && bullet.position().y == potential.y) {
            return false;
        }
        // double epsilon = std::numeric_limits<double>::epsilon();
        double epsilon = 0.0000001;

        double bullet_slope = slope(bullet.position(), potential);
        double bullet_intercept = intercept(bullet.position(), bullet_slope);

        double down1 = std::min(bullet.position().y, potential.y);
        double down2 = std::min(end1_.y, end2_.y);
        double down = std::max(down1, down2);

        double up1 = std::max(bullet.position().y, potential.y);
        double up2 = std::max(end1_.y, end2_.y);
        double up = std::min(up1, up2);

        double left1 = std::min(bullet.position().x, potential.x);
        double left2 = std::min(end1_.x, end2_.x);
        double left = std::max(left1, left2);

        double right1 = std::max(bullet.position().x, potential.x);
        double right2 = std::max(end1_.x, end2_.x);
        double right = std::min(right1, right2);

        double point_x, point_y;
        if (bullet_slope == std::numeric_limits<double>::max() && slope_ == std::numeric_limits<double>::max()) {
            return potential.x == end1_.x && up >= down;
        } else if (bullet_slope == std::numeric_limits<double>::max()) {
            point_x = potential.x;
            point_y = slope_ * point_x + intercept_;
        } else if (slope_ == std::numeric_limits<double>::max()) {
            point_x = end1_.x;
            point_y = bullet_slope * point_x + bullet_intercept;
        } else {
            point_x = - (intercept_ - bullet_intercept) / (slope_ - bullet_slope);
            point_y = bullet_slope * point_x + bullet_intercept;
        }

        // std::cout.precision(17);
        // std::cout << "potential position " << potential.x << " " << potential.y << std::endl;
        // std::cout << "person " << bullet_slope << " " << bullet_intercept << std::endl;
        // std::cout << "line   " << slope_ << " " << intercept_ << std::endl;


        // std::cout << "cross " << point_x << " " << point_y << std::endl;

        // std::cout << "up = " << up << "     down = " << down << std::endl;
        // std::cout << "left = " << left << "     right = " << down << std::endl;
        // std::cout << epsilon << std::endl;

        // std::cout << "test " << (point_y >= down - epsilon) << "   " << (point_y <= (up + epsilon)) << std::endl;
        // std::cout << "\n\n\n\n\n\n";

        if (point_x >= left - epsilon && point_x <= right + epsilon && point_y >= down - epsilon && point_y <= up + epsilon) {
            return true;
        } else {
            return false;
        }
    }
 private:
    double slope(const vec& end1, const vec& end2) const {
        if (end2.x - end1.x == 0) {
            return std::numeric_limits<double>::max();
            //return 1000000000000000000.0;
        }
        return (end2.y - end1.y) / (end2.x - end1.x);
    }
    double intercept(const vec& end, double slope) const {
        return end.y - slope * end.x;
    }
 private:
    vec end1_;
    vec end2_;
    double slope_;
    double intercept_;
};

class Block : public PhysicalObject {
 public:
    Block(const vec& position) : PhysicalObject(position, {0,0}),
                                 line1{ {position.x, position.y}, {position.x + 1, position.y} },
                                 line2{ {position.x, position.y}, {position.x, position.y + 1} },
                                 line3{ {position.x, position.y + 1}, {position.x + 1, position.y + 1} },
                                 line4{ {position.x + 1, position.y}, {position.x + 1, position.y + 1} } {
                                 // line1{ {position.x - 0.5, position.y - 0.5}, {position.x - 0.5, position.y + 0.5} },
                                 // line2{ {position.x - 0.5, position.y - 0.5}, {position.x + 0.5, position.y - 0.5} },
                                 // line3{ {position.x - 0.5, position.y + 0.5}, {position.x + 0.5, position.y + 0.5} },
                                 // line4{ {position.x + 0.5, position.y - 0.5}, {position.x + 0.5, position.y + 0.5} } {
        // int x = position.x;
        // int y = position.y;
        // vec p1 = {x - 0.5, y - 0.5};
        // vec p2 = {x - 0.5, y + 0.5};
        // vec p3 = {x + 0.5, y - 0.5};
        // vec p4 = {x + 0.5, y + 0.5};
        // line1 = Line(p1, p2};
        // line2 = Line(p1, p3);
        // line3 = Line(p2, p4);
        // line4 = Line(p3, p4);
    }

    bool collision(const PhysicalObject& object, double time) {
        return line1.collision(object, time) ||
               line2.collision(object, time) ||
               line3.collision(object, time) ||
               line4.collision(object, time);
    }
    double size() {
        return size_;
    }
 public:
    Line line1;
    Line line2;
    Line line3;
    Line line4;
    double size_ = 0.5;
};
#endif  // PHYSICAL_OBJECTS_HPP

// int main() {
//     vec vec1 = {1, 1};
//     vec vec2 = {1, 3};
//     Line l1 = {vec1, vec2}; 
//     Person point = {2, 2, -1, 0};
//     bool col = l1.collision(point, 180);
//     std::cout << col << std::endl;
// }
