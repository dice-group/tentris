#ifndef TENTRIS_SCHEMAEXCEPTION_HPP
#define TENTRIS_SCHEMAEXCEPTION_HPP

#include <exception>
#include <fmt/format.h>
#include <utility>

namespace tentris::store::graphql::exception {

	class SchemaException : public std::logic_error {

	private:
		std::string msg;

	protected:
		explicit SchemaException(const std::string &msg) : std::logic_error("GraphQL SchemaException: " + msg) {}

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
										   const std::string &field_name,
										   const std::string &parent_type)
			: SchemaException(fmt::format("Argument `{}` is not defined at field `{}` of type `{}`",
										  argument_name, field_name, parent_type)) {}
	};

	class InterfaceNotImplementedExecption : public SchemaException {

	public:
		explicit InterfaceNotImplementedExecption(const std::string &object_type,
												  const std::string &interface)
			: SchemaException(fmt::format("Type `{}` does not implement interface {}",
										  object_type, interface)) {}
	};

	class NotCompatibleFieldAliasesException : public SchemaException {

	public:
		explicit NotCompatibleFieldAliasesException(const std::string &alias)
			: SchemaException(fmt::format("Field alias `{}` applied on different types", alias)) {}
	};

	class UnsupportedTypeDirective : public SchemaException {

	public:
		UnsupportedTypeDirective(const std::string &directive,
								 const std::string &type)
			: SchemaException(fmt::format("Directive `{}` is not supported on type `{}`.", directive, type)) {}
	};

	class NotImplemented : public SchemaException {

	public:
		explicit NotImplemented(const std::string &msg) : SchemaException(msg) {}
	};


}// namespace tentris::store::graphql::exception

#endif//TENTRIS_SCHEMAEXCEPTION_HPP
