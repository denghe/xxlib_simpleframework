#ifndef __FILECACHE_H__
#define __FILECACHE_H__

// LRUCache 的单例封装与 c2dx FileUtils 注入函数 bind
// 注: 当前未并对 FileUtils::getFileData, getFileDataFromZip 予以注入( 这两个函数官方废弃中 )
class FileCache
{
public:
    MAKE_INSTANCE_H( FileCache );

    // 设置 c2dx 的回调 与 FileUtils 挂接
    FileCache();

    // 啥事都不干
    ~FileCache();

    // 清掉 cache
    void clear();

private:
    LRUCache<string, Data> _cache;
    mutex _mutex;
};

#endif

