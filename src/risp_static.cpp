#include "framework.hpp"
#include "risp.hpp"

neuro::Processor *neuro::Processor::make(const string &name, json &params)
{
  return new risp::Processor(params);
}
