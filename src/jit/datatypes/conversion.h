// Conversion between LLVM values and DataType enum values.
//
// Additionally, contains the function pointers for extracting values from a field for LLVM JIT compiled code to call
//
// Created by Max Norfolk on 5/19/23.


#pragma once

#include "jit/datatypes/types.h"
#include "jit/datatypes/conversion_declaration.h"
#include "jit/ezpsf_exceptions.h"


#include <llvm/ExecutionEngine/Orc/Core.h>
#include <llvm/ExecutionEngine/Orc/Mangling.h>

namespace fishstore::ezpsf::type_conversion {
    llvm::StringRef REF_GET_BOOL = "@__BOOL_CONV_GET";
    llvm::StringRef REF_GET_INT32 = "@__INT32_CONV_GET";
    llvm::StringRef REF_GET_INT64 = "@__INT64_CONV_GET";
    llvm::StringRef REF_GET_DOUBLE = "@__DOUBLE_CONV_GET";
    llvm::StringRef REF_GET_STR = "@__STRING_CONV_GET";

    llvm::Function *getFieldFunc(DataType type) {
        switch (type) {
            case DataType::STR_T:
                return llvm_consts::module->getFunction(REF_GET_STR);
            case DataType::INT8_T:
            LOG_RET("INT8 is not yet implemented!");
            case DataType::INT32_T:
                return llvm_consts::module->getFunction(REF_GET_INT32);
            case DataType::INT64_T:
                return llvm_consts::module->getFunction(REF_GET_INT64);
            case DataType::UINT32_T:
            case DataType::UINT64_T:
            LOG_RET("unsigned is not yet implemented!");
            case DataType::DOUBLE_T:
                return llvm_consts::module->getFunction(REF_GET_DOUBLE);
            case DataType::BOOL_T:
                return llvm_consts::module->getFunction(REF_GET_BOOL);
            case DataType::ERROR_T:
            LOG_RET("Error type detected!");
            default:
            LOG_RET("Invalid switch case!");
        }
    }


    // returns the symbol table filled function pointers so we can easily link the C/C++ methods that are used
    // to extract the fields.
    template<typename A>
    llvm::orc::SymbolMap getSymbolTable(llvm::orc::MangleAndInterner &mangle) {
        llvm::orc::SymbolMap map;

        map.insert({mangle(REF_GET_BOOL), llvm::JITEvaluatedSymbol(llvm::pointerToJITTargetAddress(
                type_conversion::TypeConversion<A>::getBool
        ), llvm::JITSymbolFlags::Exported)});

        map.insert({mangle(REF_GET_INT32), llvm::JITEvaluatedSymbol(llvm::pointerToJITTargetAddress(
                type_conversion::TypeConversion<A>::getInt32
        ), llvm::JITSymbolFlags::Exported)});

        map.insert({mangle(REF_GET_INT64), llvm::JITEvaluatedSymbol(llvm::pointerToJITTargetAddress(
                type_conversion::TypeConversion<A>::getInt64
        ), llvm::JITSymbolFlags::Exported)});

        map.insert({mangle(REF_GET_STR), llvm::JITEvaluatedSymbol(llvm::pointerToJITTargetAddress(
                type_conversion::TypeConversion<A>::getStringRef
        ), llvm::JITSymbolFlags::Exported)});

        map.insert({mangle(REF_GET_DOUBLE), llvm::JITEvaluatedSymbol(llvm::pointerToJITTargetAddress(
                type_conversion::TypeConversion<A>::getDouble
        ), llvm::JITSymbolFlags::Exported)});

        // string methods
        map.insert({mangle(str_info::REF_STR_LEN),
                    llvm::JITEvaluatedSymbol(llvm::pointerToJITTargetAddress(str_info::FUNC_STR_LEN),
                                             llvm::JITSymbolFlags::Exported)}
        );
        map.insert({mangle(str_info::REF_STR_CMP),
                    llvm::JITEvaluatedSymbol(llvm::pointerToJITTargetAddress(str_info::FUNC_STR_CMP),
                                             llvm::JITSymbolFlags::Exported)}
        );
        return map;
    }
}