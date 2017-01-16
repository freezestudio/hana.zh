
/////////////////////////////////////////////////////////////////////////////

vs2017 解析 delctype(...)::value 时会出错，只好将此语句分解成2步：
1、 using xxx = decltype(...);
2、 xxx::value;

修改处请参见：

error C2039 hana/detail/index_if.hpp : value 不是 global namespace 的成员
error C2146 hana/detail/unpack_flatten.hpp : 语法错误 缺少 > (在标识符value的前面)
error C2146 hana/detail/hash_table.hpp : 语法错误 缺少 ; (在标识符type的前面)