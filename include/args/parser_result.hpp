#ifndef ARGS_PARSER_RESULT_HPP
#define ARGS_PARSER_RESULT_HPP

#include "args/common.hpp"

ARGS_NAMESPACE_BEGIN

/**
 * @brief Parser result.
 * Object of this class is propagated all the way in parsing
 * process to indicate its status.
 */
struct parser_result {
    /**
     * @brief Indicates that parsing has succeed.
     * If it's set to false the whole parsing process
     * failed and parsed options are left in uknown
     * state. Main parser should stop its work and
     * indicate error to the user.
     */
    bool success = true;

    /**
     * @brief Next argument was consumed by parser.
     * In case of parametrized arguments (-x foo, -y 1) the
     * value may be interpreted by parser. Parsing should
     * continue after the argument.
     */
    bool consumed = false;

    /**
     * @brief Update fields to the worse state.
     * This is used internally while applying parsers.
     * Idea is to update current state accoring to incoming
     * state only if incoming state is "worse". That means:
     * if success changes to false or if consumed changes to true.
     * @param other incoming state to apply
     */
    void update(parser_result other) {
        if (other.success == false)
            success = false;
        if (other.consumed == true)
            consumed = true;
    }
};

ARGS_NAMESPACE_END

#endif
