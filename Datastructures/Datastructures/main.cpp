#include <iostream>
#include <string>
#include "LRUCache.hpp"
#include <memory>

// main entry point
int main(int argc, char *argv[]) 
{
	// create the cache
	auto cache = LRUCache<std::string, std::string>(5);

	// insert a bunch of crap into the cache
	cache.insert("1", std::shared_ptr<std::string>(new std::string("1")));
	cache.insert("2", std::shared_ptr<std::string>(new std::string("2")));
	cache.insert("3", std::shared_ptr<std::string>(new std::string("3")));
	cache.insert("4", std::shared_ptr<std::string>(new std::string("4")));
	cache.insert("5", std::shared_ptr<std::string>(new std::string("5")));
	cache.insert("6", std::shared_ptr<std::string>(new std::string("6")));
	cache.insert("7", std::shared_ptr<std::string>(new std::string("7")));

	//// call the find function
	std::cout << "7 should be in the list: " << cache.find("7").get()->c_str() << "\n";
	std::cout << "2 should not be in the list: " << cache.find("2").get() << "\n";
	std::cout << "5 should be in the list: " << cache.find("5").get()->c_str() << "\n";

	std::cout << "1 should not be in the list: " << cache["1"] << "\n";

	// just wait until something is pressed
	std::getchar();

	return 0;
}