/*
 * Key generator string -> integers modulo n
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <sstream>
#include "strzzpkeygen.h"
#include "ntlmiss.h"
//-----------------------------------------------------------------------------

StringZZpKeyGenerator::StringZZpKeyGenerator() : KeyGenerator<std::string, NTL::ZZ_p>() {
}
//-----------------------------------------------------------------------------

void StringZZpKeyGenerator::setHashAlgorithm(const HashAlgorithm<std::string>* hashAlgorithm) {
  if (hashAlgorithm == nullptr) {
    std::cerr << "setHashAlgorithm(). WARNING: null pointer given as algorithm, nothing to do." << std::endl;
    return;
  }
  
  KeyGenerator<std::string, NTL::ZZ_p>::setHashAlgorithm(hashAlgorithm);
}
//-----------------------------------------------------------------------------

void StringZZpKeyGenerator::setSecretKey(const std::string& secret) {
  this->secret = secret;
}
//-----------------------------------------------------------------------------

NTL::ZZ StringZZpKeyGenerator::getModulo() const {
  return this->p;
}
//-----------------------------------------------------------------------------

void StringZZpKeyGenerator::setModulo(const NTL::ZZ &p) {
  this->p = p;
}
//-----------------------------------------------------------------------------


NTL::ZZ_p StringZZpKeyGenerator::generate(size_t index) {
  std::stringstream ss;
  std::string derived;
  NTL::ZZ_p z;
  NTL::ZZ_pContext zzpContext;
  
  if (hashAlgorithm != nullptr) {
    ss << index++;
    derived = secret + ss.str();
    byte *hash = hashAlgorithm->hash(derived);
    
    zzpContext.save();
    NTL::ZZ_p::init(this->p);
    conv(z, NTL::ZZFromBytes(hash, hashAlgorithm->hashSize()));
    zzpContext.restore();
  } else {
    std::cerr << "setSecretKey(). Warning: a hash algorithm must be set first." << std::endl;
  }
  
  return z;
}
//-----------------------------------------------------------------------------
