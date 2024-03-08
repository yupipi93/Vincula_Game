#include <vector>
#include <unordered_map>
#include <iterator>
//console.log



template <class N, class T>
struct linearMap {

    using vector_t = typename std::vector< std::pair<N, T> >;
    using iterator = typename vector_t::iterator;
    using const_iterator = typename vector_t::const_iterator;

    explicit linearMap() {
    }


    explicit linearMap(size_t size) {
        vec.reserve(size);
        map.reserve(size);
    }

    void reserve(size_t size) {
        vec.reserve(size);
        map.reserve(size);
    }

    void emplace(N id, T element) {
        vec.emplace_back(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(element));
        map.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(vec.size() - 1));
    }

    void emplace(N id) {
        vec.emplace_back();
        map.emplace(id, vec.size() - 1);
    }

    T &at(N id) {
        uint32_t index = 0;
        //try {
        index = map.at(id);
        //} catch(std::exception e) {
        //}
        return vec[index].second;

    }



    void erase(N id) {

        if (map.find(id) != map.end()) {
            if(id != vec[vec.size() - 1].first) {
                //get index to delete
                int32_t index = map.at(id);
                //swap index element with last element
                vec[index] = vec[vec.size() - 1]; //swap pair
                //delete last element
                vec.pop_back();
                //delete element in map
                map.erase(id);
                //update index in moved element
                map.at(vec[index].first) = index;

            } else {
                //element to erase its the last element
                vec.pop_back();
                map.erase(id);
            }


        } 

    }


    void clear() {
        map.clear();
        vec.clear();
    }

    size_t size() const {
        return vec.size();
    }

    size_t capacity() const {
        return vec.capacity();
    }

    size_t count(const N id) const {
        return map.count(id);
    }



    std::unordered_map<N, uint32_t> getMap() {
        return map;
    }



    //##### Iterators
    iterator begin() {
        return vec.begin();
    }
    iterator end() {
        return vec.end();
    }
    const_iterator begin() const {
        return vec.begin();
    }
    const_iterator end() const {
        return vec.end();
    }
    const_iterator cbegin() const {
        return vec.cbegin();
    }
    const_iterator cend() const {
        return vec.cend();
    }


    const_iterator find(N id) const {
        if (map.find(id) != map.end()) {
            uint32_t index = map.at(id);
            return vec.cbegin() + index;
        }
        //element not found
        return vec.cend();
    }



private:
    vector_t vec;
    std::unordered_map<N, uint32_t> map;

};
