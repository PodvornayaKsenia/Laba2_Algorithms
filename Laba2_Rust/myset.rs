use crate::array::DynArray;
use crate::single_list::SingleList;
pub struct MySet { buckets: DynArray<SingleList>, size: usize }
impl MySet {
    pub fn new() -> Self { Self::with_capacity(16) }
    fn with_capacity(capacity: usize) -> Self {
        let mut buckets = DynArray::with_capacity(capacity);
        for _ in 0..capacity { buckets.push(SingleList::new()); }
        Self { buckets, size: 0 }
    }
    fn hash(value: &str, capacity: usize) -> usize {
        let mut h = 0u64;
        for byte in value.bytes() { h = h.wrapping_mul(31).wrapping_add(byte as u64); }
        (h % capacity as u64) as usize
    }
    pub fn contains(&self, value: &str) -> bool {
        self.buckets[Self::hash(value, self.buckets.len())].search(value)
    }
    fn resize(&mut self) {
        let new_capacity = self.buckets.len() * 2;
        let mut old = std::mem::replace(&mut self.buckets, {
            let mut new_buckets = DynArray::with_capacity(new_capacity);
            for _ in 0..new_capacity { new_buckets.push(SingleList::new()); }
            new_buckets
        });
        for i in 0..old.len() {
            while let Some(value) = old[i].pop_front() {
                let index = Self::hash(&value, new_capacity);
                self.buckets[index].push_front(value);
            }
        }
    }
    pub fn insert(&mut self, value: &str) -> bool {
        if self.contains(value) { return false; }
        if (self.size + 1) * 4 > self.buckets.len() * 3 { self.resize(); }
        let index = Self::hash(value, self.buckets.len());
        self.buckets[index].push_front(value.to_string());
        self.size += 1;
        true
    }
    pub fn remove(&mut self, value: &str) -> bool {
        let index = Self::hash(value, self.buckets.len());
        if self.buckets[index].remove_by_value(value) { self.size -= 1; true } else { false }
    }
    pub fn for_each(&self, mut f: impl FnMut(&str)) {
        for i in 0..self.buckets.len() { self.buckets[i].for_each(&mut f); }
    }
    pub fn display(&self) -> String {
        let mut text = String::from("{");
        let mut first = true;
        self.for_each(|value| {
            if !first { text.push_str(", "); }
            text.push_str(value);
            first = false;
        });
        text.push('}');
        text
    }
}
impl Default for MySet { fn default() -> Self { Self::new() } }
