#ifndef DICE_SPARQL_UPDATEQUERY_HPP
#define DICE_SPARQL_UPDATEQUERY_HPP

#include <dice/rdf-tensor/HypertrieTrait.hpp>


namespace dice::sparql2tensor {

	struct UPDATEDATAQueryData {
		rdf_tensor::parser::IStreamQuadIterator::prefix_storage_type prefixes;
		bool is_delete; // is this query DELETE DATA? (otherwise is INSERT DATA)
		std::vector<rdf_tensor::NonZeroEntry> entries;

		static UPDATEDATAQueryData parse(std::string_view sparql_update_str);
	};
}

#endif//DICE_SPARQL_UPDATEQUERY_HPP
