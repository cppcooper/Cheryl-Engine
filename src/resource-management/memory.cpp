#include <resource-management/memory.h>
using namespace CherylE;

void* MemoryMgr::get(size_t bytes) {
    return get(bytes,default_alloc_size);
}

void* MemoryMgr::get(size_t bytes, size_t alloc_size) {
    size_t remainder = 0;
    uint64_t ptr = nullptr;
    auto iter = OpenList.lower_bound(bytes);
    if(iter != OpenList.end()){
        remainder = iter->first - bytes;
        auto ptr_pair = iter->second;
        ptr = ptr_pair.second;
        OpenList.erase(iter);
        if(remainder>0){
            OpenList.emplace(remainder,ptr+bytes);
        }
        ClosedList.emplace(ptr_pair.first,std::make_pair(ptr,bytes));
    }
    else{
        remainder = bytes >= alloc_size ? 0 : alloc_size - bytes;
        ptr = malloc(remainder != 0 ? alloc_size : bytes);
        if (!ptr) {
            throw bad_alloc(__FUNCTION__,__LINE__);
        }
        MasterRecord.emplace(ptr);
        if(remainder>0){
            OpenList.emplace(remainder,ptr+bytes);
        }
        ClosedList.emplace(ptr,std::make_pair(ptr,bytes));
    }
    return (void*)ptr;
}



void MemoryMgr::put(void *ptr, size_t bytes) {

}

