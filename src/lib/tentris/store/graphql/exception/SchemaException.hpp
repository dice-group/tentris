#ifndef TENTRIS_SCHEMAEXCEPTION_HPP
#define TENTRIS_SCHEMAEXCEPTION_HPP

#include <exception>
#include <fmt/format.h>
#include <utility>

namespace tentris::store::graphql::exception {

	class SchemaException : public std::invalid_argument {

	private:
		std::string msg;

	protected:
		explicit SchemaException(const std::string &msg) : invalid_argument("GraphQL SchemaException: " + msg) {}

	public:
		~SchemaException() noexcept override = default;
	};

	class FieldNotFoundException : public SchemaException {

	public:
		FieldNotFoundException(const std::string &field_name,
							   const std::string &parent_type)
			: SchemaException(fmt::format("Field `{}` is not part of type `{}`", field_name, parent_type)) {}
	};

	class TypeNotFoundException : public SchemaException {

	public:
		explicit TypeNotFoundException(const std::string &field_name)
			: SchemaException(fmt::format("Type `{}` is not part of the schema", field_name)) {}
	};

	class ArgumentNotFoundException : public SchemaException {

	public:
		explicit ArgumentNotFoundException(const std::string &argument_name,
										   const std::string &type,
										   const std::string &name)
			: SchemaException(fmt::format("Argument `{}` was not provided at {}: {}",
										  argument_name, type, name)) {}
	};

}// namespace tentris::store::graphql::exception

#endif//TENTRIS_TENTRIS_SCHEMAEXCEPTION_HPP_HPP
