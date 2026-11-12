use crate::double_list::DoubleList;
pub struct LRUCache { list: DoubleList, capacity: usize, size: usize }
impl LRUCache {
    pub fn new(capacity: usize) -> Self { Self { list: DoubleList::new(), capacity, size: 0 } }
    pub fn set(&mut self, key: i32, value: i32) {
        if self.capacity == 0 { return; }
        if let Some(old) = self.list.find_key(key) {
            self.list.remove_by_value(&old);
            self.list.push_back(format!("{key} {value}"));
            return;
        }
        if self.size == self.capacity { self.list.pop_front(); self.size -= 1; }
        self.list.push_back(format!("{key} {value}"));
        self.size += 1;
    }
    pub fn get(&mut self, key: i32) -> i32 {
        if let Some(data) = self.list.find_key(key) {
            let value = data.split_whitespace().nth(1).and_then(|v| v.parse().ok()).unwrap_or(-1);
            self.list.remove_by_value(&data);
            self.list.push_back(data);
            value
        } else { -1 }
    }
}
