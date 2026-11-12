use crate::array::Array;
use std::cell::Cell;

fn hash1(key: &str) -> u64 {
    let mut h = 14695981039346656037u64;
    for b in key.bytes() { h ^= b as u64; h = h.wrapping_mul(1099511628211); }
    h
}
fn hash2(key: &str) -> u64 {
    let mut h = 5381u64;
    for b in key.bytes() { h = h.wrapping_mul(33).wrapping_add(b as u64); }
    h
}
pub struct BloomFilter { bits: Array, count: usize }
impl BloomFilter {
    pub fn new(count: usize) -> Self {
        assert!(count > 0);
        let mut bits = Array::new();
        for _ in 0..count { bits.push("0".into()); }
        Self { bits, count }
    }
    pub fn add(&mut self, key: &str) {
        let h1 = hash1(key);
        let h2 = hash2(key) | 1;
        for i in 0..3u64 {
            let index = (h1.wrapping_add(i.wrapping_mul(h2)) % self.count as u64) as usize;
            self.bits.replace(index, "1".into());
        }
    }
    pub fn possibly_contains(&self, key: &str) -> bool {
        let h1 = hash1(key);
        let h2 = hash2(key) | 1;
        for i in 0..3u64 {
            let index = (h1.wrapping_add(i.wrapping_mul(h2)) % self.count as u64) as usize;
            if self.bits.get(index) == Some("0") { return false; }
        }
        true
    }
}
pub struct HashTable {
    keys: Array,
    values: Array,
    states: Array,
    filter: BloomFilter,
    capacity: usize,
    count: usize,
    double_hashing: bool,
    probes: Cell<u64>,
}
impl HashTable {
    pub fn new(double_hashing: bool, capacity: usize) -> Self {
        assert!(capacity >= 2);
        let mut keys = Array::new();
        let mut values = Array::new();
        let mut states = Array::new();
        for _ in 0..capacity { keys.push(String::new()); values.push(String::new()); states.push("0".into()); }
        Self { keys, values, states, filter: BloomFilter::new(8192), capacity, count: 0, double_hashing, probes: Cell::new(0) }
    }
    fn position(&self, key: &str, attempt: usize) -> usize {
        let start = hash1(key) % self.capacity as u64;
        let step = if self.double_hashing { 1 + hash2(key) % (self.capacity as u64 - 1) } else { 1 };
        (start.wrapping_add((attempt as u64).wrapping_mul(step)) % self.capacity as u64) as usize
    }
    fn count_probe(&self) { self.probes.set(self.probes.get() + 1); }
    fn find_index(&self, key: &str) -> Option<usize> {
        for i in 0..self.capacity {
            let index = self.position(key, i);
            self.count_probe();
            let state = self.states.get(index);
            if state == Some("0") { return None; }
            if state == Some("1") && self.keys.get(index) == Some(key) { return Some(index); }
        }
        None
    }
    pub fn insert(&mut self, key: &str, value: &str) -> bool {
        let mut first_deleted = None;
        for i in 0..self.capacity {
            let index = self.position(key, i);
            self.count_probe();
            match self.states.get(index) {
                Some("1") if self.keys.get(index) == Some(key) => {
                    self.values.replace(index, value.to_string());
                    return false;
                }
                Some("2") => { if first_deleted.is_none() { first_deleted = Some(index); } }
                Some("0") => {
                    self.place(first_deleted.unwrap_or(index), key, value);
                    return true;
                }
                _ => {}
            }
        }
        if let Some(index) = first_deleted { self.place(index, key, value); return true; }
        false
    }
    fn place(&mut self, index: usize, key: &str, value: &str) {
        self.keys.replace(index, key.to_string());
        self.values.replace(index, value.to_string());
        self.states.replace(index, "1".into());
        self.count += 1;
        self.filter.add(key);
    }
    pub fn get(&self, key: &str) -> Option<&str> {
        if !self.filter.possibly_contains(key) { return None; }
        self.find_index(key).and_then(|i| self.values.get(i))
    }
    pub fn contains(&self, key: &str) -> bool { self.get(key).is_some() }
    pub fn contains_without_bloom(&self, key: &str) -> bool { self.find_index(key).is_some() }
    pub fn remove(&mut self, key: &str) -> bool {
        if !self.filter.possibly_contains(key) { return false; }
        let index = match self.find_index(key) { Some(i) => i, None => return false };
        self.keys.replace(index, String::new());
        self.values.replace(index, String::new());
        self.states.replace(index, "2".into());
        self.count -= 1;
        true
    }
    pub fn bloom_check(&self, key: &str) -> bool { self.filter.possibly_contains(key) }
    pub fn capacity(&self) -> usize { self.capacity }
    pub fn probes(&self) -> u64 { self.probes.get() }
    pub fn reset_probes(&self) { self.probes.set(0); }
    pub fn print(&self) {
        for i in 0..self.capacity {
            if self.states.get(i) == Some("1") {
                println!("[{i}] {}: {}", self.keys.get(i).unwrap_or(""), self.values.get(i).unwrap_or(""));
            }
        }
    }
}
