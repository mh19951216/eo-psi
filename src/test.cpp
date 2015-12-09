#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <NTL/ZZ_p.h>

#include "hashbuckets.h"
#include "murmurhash3.h"
#include "strint.h"
#include "simplebm.h"

#include "test.h"
//-----------------------------------------------------------------------------

/**
 * 1 - Arguments: file name, length (number of buckets) and max load for a bucket
 *     the file contains big integers modulo p
 * 2 - Create hash table
 *     a) hash each number and add to the hash table
 *     b) benchmark the insertion
 * 3 - output the size of buckets (uniformity is a must)
 */
int main(int argc, char **argv) {
  HashBuckets<NTL::ZZ_p>* hashBuckets;
  HashAlgorithm<NTL::ZZ_p>* hashAlgorithm;
  std::string infilename = DEFAULT_FILENAME;
  std::ifstream infile;
  size_t maxLoad = DEFAULT_HASHBUCKETS_MAXLOAD;
  size_t length = DEFAULT_HASHBUCKETS_LENGTH;
  size_t n;
  NTL::ZZ_p *z;
  NTL::ZZ p;
  SimpleBenchmark benchmark;
  
  // Initialize numbers modulo p
  p = str2zz(DEFAULT_P);
  NTL::ZZ_p::init(p);
  
  // Use a specific seed to generate the same hashes
  hashAlgorithm = new MurmurHash3(DEFAULT_MURMURHASH_SEED);
  hashBuckets = new HashBuckets<NTL::ZZ_p>(length, maxLoad);
  if (hashBuckets == nullptr) { // or is a try-catch more appropriate?
    std::cerr << argv[0] << ". Error allocating hash table." << std::endl;
    exit(1);
  }
  hashBuckets->setHashAlgorithm(hashAlgorithm);
  
  // Open file with numbers
  if (argc >= 2) {
    infilename = argv[1];
  } else {
    std::cerr << argv[0] << ". WARNING: using default input file name \"" << infilename << "\"." << std::endl;
  }
  infile.open(infilename, std::ifstream::in);
  if (infile.fail()) {
    std::cerr << argv[0] << ". Error opening file \"" << infilename << "\"" << std::endl;
    exit(1);
  }
  
  // Read all numbers at once (?)
  infile >> n;
  z = new NTL::ZZ_p[n];
  if (z == nullptr) {
    std::cerr << argv[0] << ". Error loading all " << n << " numbers to memory." << std::endl;
    exit(1);
  }
  for (size_t i = 0; i < n; i++) {
    infile >> z[i];
  }
  
  // Add to hash
  benchmark.start();
  for (size_t i = 0; i < n; i++) {
    hashBuckets->add(z[i]);
  }
  benchmark.stop();
  
  // Print stats
  hashBuckets->printStats();
  std::cout << "Total time to add " << n << " elements to the hashtable: " << benchmark.benchmark().count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to add an element to the hashtable: " << (double) benchmark.benchmark().count() / n << " µs" << std::endl;
  
  delete(hashAlgorithm);
  delete(hashBuckets);
  return 0;
}
//-----------------------------------------------------------------------------
