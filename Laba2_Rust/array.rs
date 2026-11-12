use std::ops::{Index, IndexMut};

pub struct DynArray<T> {
    data: Box<[Option<T>]>,
    len: usize,
}
impl<T> DynArray<T> {
    pub fn new() -> Self { Self::with_capacity(4) }
    pub fn with_capacity(capacity: usize) -> Self {
        Self { data: std::iter::repeat_with(|| None).take(capacity.max(1)).collect(), len: 0 }
    }
    pub fn len(&self) -> usize { self.len }
    pub fn is_empty(&self) -> bool { self.len == 0 }
    fn grow(&mut self) {
        let mut next: Box<[Option<T>]> = std::iter::repeat_with(|| None).take(self.data.len() * 2).collect();
        for i in 0..self.len { next[i] = self.data[i].take(); }
        self.data = next;
    }
    pub fn push(&mut self, value: T) {
        if self.len == self.data.len() { self.grow(); }
        self.data[self.len] = Some(value);
        self.len += 1;
    }
    pub fn get(&self, index: usize) -> Option<&T> {
        if index < self.len { self.data[index].as_ref() } else { None }
    }
    pub fn get_mut(&mut self, index: usize) -> Option<&mut T> {
        if index < self.len { self.data[index].as_mut() } else { None }
    }
    pub fn replace(&mut self, index: usize, value: T) {
        if index < self.len { self.data[index] = Some(value); }
    }
    pub fn insert(&mut self, index: usize, value: T) {
        if index > self.len { return; }
        if self.len == self.data.len() { self.grow(); }
        for i in (index..self.len).rev() { self.data[i+1] = self.data[i].take(); }
        self.data[index] = Some(value);
        self.len += 1;
    }
    pub fn remove(&mut self, index: usize) -> Option<T> {
        if index >= self.len { return None; }
        let removed = self.data[index].take();
        for i in index..self.len-1 { self.data[i] = self.data[i+1].take(); }
        self.len -= 1;
        self.data[self.len] = None;
        removed
    }
    pub fn clear(&mut self) { while self.len > 0 { self.len -= 1; self.data[self.len] = None; } }
    pub fn iter(&self) -> impl Iterator<Item=&T> { self.data[..self.len].iter().map(|x| x.as_ref().expect("Элемент массива")) }
}
impl<T> Default for DynArray<T> { fn default() -> Self { Self::new() } }
impl<T> Index<usize> for DynArray<T> {
    type Output = T;
    fn index(&self, index: usize) -> &T { self.get(index).expect("Индекс вне массива") }
}
impl<T> IndexMut<usize> for DynArray<T> {
    fn index_mut(&mut self, index: usize) -> &mut T { self.get_mut(index).expect("Индекс вне массива") }
}

pub struct Array { inner: DynArray<String> }
impl Array {
    pub fn new() -> Self { Self { inner: DynArray::new() } }
    pub fn init(&mut self, capacity: usize) { self.inner = DynArray::with_capacity(capacity); }
    pub fn push(&mut self, value: String) { self.inner.push(value); }
    pub fn insert(&mut self, index: usize, value: String) { self.inner.insert(index, value); }
    pub fn get(&self, index: usize) -> Option<&str> { self.inner.get(index).map(String::as_str) }
    pub fn remove(&mut self, index: usize) { self.inner.remove(index); }
    pub fn replace(&mut self, index: usize, value: String) { self.inner.replace(index, value); }
    pub fn len(&self) -> usize { self.inner.len() }
    pub fn is_empty(&self) -> bool { self.inner.is_empty() }
    pub fn iter(&self) -> impl Iterator<Item=&String> { self.inner.iter() }
}
impl Default for Array { fn default() -> Self { Self::new() } }
