#ifndef TENTRIS_GRAPHQLSCHEMAENDPOINT_HPP
#define TENTRIS_GRAPHQLSCHEMAENDPOINT_HPP

#include <restinio/all.hpp>
#include <restinio/helpers/file_upload.hpp>
#include <restinio/helpers/multipart_body.hpp>
#include <sstream>
#include <string>

namespace tentris::http::graphql_schema_endpoint {

	struct GraphQLSchemaEndpoint {

		auto operator()(const restinio::request_handle_t &req,
						[[maybe_unused]] auto params) -> restinio::request_handling_status_t {
			using namespace restinio::file_upload;
			using namespace tentris::store;
			tentris::logging::log("GraphQLSchemaEndpoint | request started");
			auto resp = req->create_response<restinio::restinio_controlled_output_t>();
			resp.append_header(restinio::http_field::content_type, "text/plain");
			ResultState status = ResultState::OK;
			const auto result =
					enumerate_parts_with_files(
							*req,
							[&resp, &status](const part_description_t &part) {
								if (part.name != "schema") {
									return handling_result_t::continue_enumeration;
								} else {
									try {
										tentris::logging::log(fmt::format("GraphQLSchemaEndpoint | Processing: {}",
																		  part.filename.value()));
										graphql::GraphQLParser::parseSchema(std::string{part.body},
																			AtomicGraphQLSchema::getInstance());
										resp.set_body("Schema successfully updated!");
									} catch (std::exception &e) {
										resp.set_body(e.what());
										status = ResultState::UNPARSABLE;
										tentris::logging::logError(fmt::format("GraphQLSchemaEndpoint | {}", e.what()));
									}
									return handling_result_t::stop_enumeration;
								}
							});
			if (not result) {
				resp.set_body("Please provide the schema using the parameter: `schema`");
				status = ResultState::UNPARSABLE;
                tentris::logging::logError("GraphQLSchemaEndpoint | Parameter `schema` not provided");
			}
			resp.done();
			if (status == ResultState::UNPARSABLE)
				return restinio::request_rejected();
			tentris::logging::log("GraphQLSchemaEndpoint | Schema updated");
			return restinio::request_accepted();
		}
	};

}// namespace tentris::http::graphql_schema_endpoint

#endif//TENTRIS_GRAPHQLSCHEMAENDPOINT_HPP
