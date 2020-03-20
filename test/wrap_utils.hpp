#ifndef WFD_WRAP_UTILS_HPP
#define WFD_WRAP_UTILS_HPP

#define WFD_WRAP_FUNC0( GLOBAL_VAR, RETURN_TYPE, FUNC_NAME ) \
    extern RETURN_TYPE __real_ ## FUNC_NAME (); \
    RETURN_TYPE __wrap_ ## FUNC_NAME () \
    { \
        if (nullptr == GLOBAL_VAR ) \
        { \
            return __real_ ## FUNC_NAME (); \
        } \
        else \
        { \
            return GLOBAL_VAR -> FUNC_NAME(); \
        } \
    }

#define WFD_WRAP_FUNC1( GLOBAL_VAR, RETURN_TYPE, FUNC_NAME, ARG1_TYPE ) \
    extern RETURN_TYPE __real_ ## FUNC_NAME (ARG1_TYPE); \
    RETURN_TYPE __wrap_ ## FUNC_NAME (ARG1_TYPE arg1) \
    { \
        if (nullptr == GLOBAL_VAR ) \
        { \
            return __real_ ## FUNC_NAME (arg1); \
        } \
        else \
        { \
            return GLOBAL_VAR -> FUNC_NAME(arg1); \
        } \
    }

#define WFD_WRAP_FUNC2( GLOBAL_VAR, RETURN_TYPE, FUNC_NAME, ARG1_TYPE, ARG2_TYPE ) \
    extern RETURN_TYPE __real_ ## FUNC_NAME (ARG1_TYPE, ARG2_TYPE); \
    RETURN_TYPE __wrap_ ## FUNC_NAME (ARG1_TYPE arg1, ARG2_TYPE arg2) \
    { \
        if (nullptr == GLOBAL_VAR ) \
        { \
            return __real_ ## FUNC_NAME (arg1, arg2); \
        } \
        else \
        { \
            return GLOBAL_VAR -> FUNC_NAME(arg1, arg2); \
        } \
    }

#endif
