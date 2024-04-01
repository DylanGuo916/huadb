#include "table/table_scan.h"

#include "table/table_page.h"

namespace huadb {

TableScan::TableScan(BufferPool &buffer_pool, std::shared_ptr<Table> table, Rid rid)
    : buffer_pool_(buffer_pool), table_(std::move(table)), rid_(rid) {}

std::shared_ptr<Record> TableScan::GetNextRecord(xid_t xid, IsolationLevel isolation_level, cid_t cid,
                                                 const std::unordered_set<xid_t> &active_xids) {
  // 根据事务隔离级别及活跃事务集合，判断记录是否可见
  // LAB 3 BEGIN

  // 每次调用读取一条记录
  // 读取时更新 rid_ 变量，避免重复读取
  // 扫描结束时，返回空指针
  // LAB 1 BEGIN
  // TODO:
  /*** 每次获取记录时都创建一个新的TablePage实例可能不是最高效的做法。
   * 考虑是否可以重用TablePage实例或者采用其他方式减少重复的页面加载操作，
   * 特别是在连续读取同一页面上的多条记录时。 ***/
  while (true) {
    if (rid_.page_id_ == NULL_PAGE_ID) return nullptr;
    auto table_page = std::make_unique<TablePage>(buffer_pool_.GetPage(table_->GetDbOid(), table_->GetOid(), rid_.page_id_));
    std::shared_ptr<Record> record = nullptr;
    record = table_page->GetRecord(rid_.slot_id_, table_->GetColumnList());
    rid_.slot_id_ += 1;
    if (rid_.slot_id_ == table_page->GetRecordCount()) {
      if (table_page->GetNextPageId() == NULL_PAGE_ID) {rid_.page_id_ = NULL_PAGE_ID;} 
      else {rid_.page_id_ += 1;}
      rid_.slot_id_ = 0;
    }
    if (record->IsDeleted()) continue;
    std::shared_ptr<Record> ret = std::move(record);
    return ret;
  }
}

}  // namespace huadb
