#ifndef TENTRIS_QUERYRESULTSTATE_HPP
#define TENTRIS_QUERYRESULTSTATE_HPP

namespace tentris::http {
	enum ResultState {
		OK, UNPARSABLE, UNKNOWN_REQUEST, PROCESSING_TIMEOUT, SERIALIZATION_TIMEOUT, UNEXPECTED, SEVERE_UNEXPECTED,
	};

	std::ostream &operator<<(std::ostream &os, ResultState state) {
		switch (state) {
			case OK :
				return os << "OK";
			case UNPARSABLE :
				return os << "UNPARSABLE";
			case UNKNOWN_REQUEST :
				return os << "UNKNOWN_REQUEST";
			case PROCESSING_TIMEOUT :
				return os << "PROCESSING_TIMEOUT";
			case SERIALIZATION_TIMEOUT :
				return os << "SERIALIZATION_TIMEOUT";
			case UNEXPECTED :
				return os << "UNEXPECTED";
			case SEVERE_UNEXPECTED :
				return os << "SEVERE_UNEXPECTED";
			default:
				return os << static_cast<std::uint16_t>(state);
		}
	}
}

#endif //TENTRIS_QUERYRESULTSTATE_HPP
