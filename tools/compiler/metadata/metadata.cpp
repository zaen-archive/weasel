//
// Created by zaen on 27/06/21.
//
#include "weasel/metadata/metadata.h"
#include "weasel/config/config.h"

weasel::Metadata::Metadata(llvm::LLVMContext *context) {
    _context = context;
    _builder = new llvm::MDBuilder(*context);
}

llvm::Metadata *weasel::Metadata::getFlagsMetadata()
{
    return getBuilder()->createConstant(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*getContext()), 4));
}

llvm::Metadata *weasel::Metadata::getVersionMetadata()
{
    return getBuilder()->createString(WEASEL_FULL_VERSION);
}

llvm::MDNode *weasel::Metadata::getCLVersionMetadata()
{
    auto metas = std::vector<llvm::Metadata *>{
            getBuilder()->createConstant(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*getContext()), 2)),
            getBuilder()->createConstant(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*getContext()), 0)),
    };
    return llvm::MDNode::get(*getContext(), metas);
}
