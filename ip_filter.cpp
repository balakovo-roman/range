#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

#include <range/v3/algorithm/sort.hpp>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/iterator/stream_iterators.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/algorithm/upper_bound.hpp>
#include <range/v3/algorithm/lower_bound.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/view/filter.hpp>

using ip_byte = unsigned char;
using ip_address = std::array<ip_byte, 4>;
using ip_list = std::vector<ip_address>;
using ip_list_ci = ip_list::const_iterator;

ip_address get_ip(std::string& str)
{
    std::string::size_type sz{ 0 };
    ip_address temp{};
    temp[0] = std::stoi(str, &sz);
    temp[1] = std::stoi(str.erase(0, sz + 1), &sz);
    temp[2] = std::stoi(str.erase(0, sz + 1), &sz);
    temp[3] = std::stoi(str.erase(0, sz + 1));
    return temp;
}

void input_ip_list(ip_list& ip_pool) {
    for (std::string line; std::getline(std::cin, line);) {
        ip_pool.emplace_back(get_ip(line));
    }
}

std::ostream& operator<<(std::ostream& ostream, const ip_address& ipAddress) {
    for (int i = 0; i < 4; i++)
        ostream << (int)ipAddress[i] << ((i < 3) ? "." : "");
    return ostream;
}

std::ostream& operator<<(std::ostream& ostream, const ip_list &ipList) {
    ranges::copy(ipList, ranges::ostream_iterator<ip_address>(ostream, "\n"));
    return ostream;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    try
    {
        ip_list ipPool;
        input_ip_list(ipPool);
        ipPool.shrink_to_fit();

        // TODO reverse lexicographically sort
        ranges::sort(ipPool, ranges::greater());
        std::cout << ipPool;

        // TODO filter by first byte and output
        auto filter = [&ipPool](auto&&... args) {
            auto ipAddress = ip_address{ (ip_byte)args... };
            auto ipAddress2 = ip_address{ (ip_byte)args... };
            for (int i = sizeof...(args); i < 4; i++) {
                ipAddress[i] = 0;
                ipAddress2[i] = 255;
            }
            auto it_l = ranges::lower_bound(ipPool, ipAddress, ranges::greater());
            auto it_u = ranges::upper_bound(ipPool, ipAddress2, ranges::greater());
            ranges::copy(it_u, it_l, ranges::ostream_iterator<ip_address>(std::cout, "\n"));
        };
        filter(1);

        // TODO filter by first and second bytes and output
        filter(46, 70);

        // TODO filter by any byte and output
        auto filter_any = [&ipPool](const ip_byte ipByte) {
            ranges::for_each(ipPool | ranges::views::filter([ipByte](const ip_address& ipAddress) {
                return ranges::any_of(ipAddress, [ipByte](const ip_byte& val) {
                    return val == ipByte;
                    });
                }), 
                [](const ip_address& ipAddress) {
                    std::cout << ipAddress << std::endl; 
                });
        };
        filter_any(46);

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
