#include <Rcpp.h>
#include "utils.h"
using namespace Rcpp;

std::string type_name(SEXP x) {
  switch (TYPEOF(x)) {
  case NILSXP:      return "NULL";
  case SYMSXP:      return "symbol";
  case LISTSXP:     return "pairlist";
  case CLOSXP:      return "function";
  case ENVSXP:      return "environment";
  case PROMSXP:     return "promise";
  case LANGSXP:     return "call";
  case SPECIALSXP:  return "special";
  case BUILTINSXP:  return "builtin";
  case CHARSXP:     return "string";
  case LGLSXP:      return "logical";
  case INTSXP:      return "integer";
  case REALSXP:     return "double";
  case CPLXSXP:     return "complex";
  case STRSXP:      return "character";
  case DOTSXP:      return "...";
  case ANYSXP:      return "any";
  case VECSXP:      return "list";
  case EXPRSXP:     return "expression";
  case BCODESXP:    return "bytecode";
  case EXTPTRSXP:   return "external pointer";
  case WEAKREFSXP:  return "weak ref";
  case RAWSXP:      return "raw";
  case S4SXP:       return "S4";
  default:          return "<unknown>";
  }
}

std::string prim_type_(RObject x) {
  std::string type = type_name(x);

  RObject klass = x.attr("class");
  if (klass == R_NilValue)
    return type;

  type += " (";
  if (IS_S4_OBJECT(x)) {
    if (TYPEOF(x) != S4SXP)
      type += "S4: ";

    RObject package = klass.attr("package");
    if (package != R_NilValue) {
      type += as<std::string>(package) + "::";
    }

    type += as<std::string>(klass);
  } else {
    type += "S3: ";

    CharacterVector klasses = as<CharacterVector>(klass);

    for (int i = 0; i < klasses.size(); ++i) {
      type += klasses[i];
      if (i != klasses.size() - 1)
        type += ", ";
    }

  }
  type += ")";

  return type;
}

// [[Rcpp::export]]
std::string prim_type_(RObject name, Environment env) {
  return prim_type_(find_var(name, env));
}
