#define DUCKDB_EXTENSION_MAIN

#include "toml_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/function/scalar_function.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void TomlScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "...........🦆 " + name.GetString());
	});
}

inline void TomlOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Toml " + name.GetString() + ", my linked OpenSSL version is " +
		                                           OPENSSL_VERSION_TEXT);
	});
}

static void LoadInternal(ExtensionLoader &loader) {
	// Register a scalar function
	auto toml_scalar_function =
	    ScalarFunction("toml", {LogicalType::VARCHAR}, LogicalType::VARCHAR, TomlScalarFun);

	loader.RegisterFunction(toml_scalar_function);

	// Register another scalar function
	auto toml_openssl_version_scalar_function = ScalarFunction("toml_openssl_version", {LogicalType::VARCHAR},
	                                                             LogicalType::VARCHAR, TomlOpenSSLVersionScalarFun);
	loader.RegisterFunction(toml_openssl_version_scalar_function);
}

void TomlExtension::Load(ExtensionLoader &loader) {
	LoadInternal(loader);
}
std::string TomlExtension::Name() {
	return "toml";
}

std::string TomlExtension::Version() const {
#ifdef EXT_VERSION_TOML
	return EXT_VERSION_TOML;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_CPP_EXTENSION_ENTRY(toml, loader) {
	duckdb::LoadInternal(loader);
}
}
