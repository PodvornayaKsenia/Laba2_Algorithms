use std::cell::RefCell;
use std::rc::{Rc, Weak};
type Link = Option<Rc<RefCell<DNode>>>;
pub struct DNode {
    pub data: String,
    pub next: Link,
    pub prev: Option<Weak<RefCell<DNode>>>,
}
#[derive(Default)]
pub struct DoubleList { head: Link, tail: Link }
impl DoubleList {
    pub fn new() -> Self { Self { head: None, tail: None } }
    pub fn push_back(&mut self, value: String) {
        let node = Rc::new(RefCell::new(DNode { data: value, next: None, prev: None }));
        if let Some(old) = self.tail.take() {
            node.borrow_mut().prev = Some(Rc::downgrade(&old));
            old.borrow_mut().next = Some(node.clone());
            self.tail = Some(node);
        } else {
            self.head = Some(node.clone());
            self.tail = Some(node);
        }
    }
    pub fn pop_front(&mut self) -> Option<String> {
        let node = self.head.take()?;
        let data = node.borrow().data.clone();
        let next = node.borrow_mut().next.take();
        self.head = next;
        if let Some(ref head) = self.head { head.borrow_mut().prev = None; }
        else { self.tail = None; }
        Some(data)
    }
    pub fn remove_by_value(&mut self, value: &str) -> bool {
        let mut current = self.head.clone();
        while let Some(node) = current {
            if node.borrow().data == value {
                let prev = node.borrow().prev.as_ref().and_then(Weak::upgrade);
                let next = node.borrow().next.clone();
                if let Some(ref p) = prev { p.borrow_mut().next = next.clone(); }
                else { self.head = next.clone(); }
                if let Some(ref n) = next { n.borrow_mut().prev = prev.as_ref().map(Rc::downgrade); }
                else { self.tail = prev; }
                node.borrow_mut().next = None;
                node.borrow_mut().prev = None;
                return true;
            }
            current = node.borrow().next.clone();
        }
        false
    }
    pub fn find_key(&self, key: i32) -> Option<String> {
        let key = key.to_string();
        let mut current = self.head.clone();
        while let Some(node) = current {
            let borrow = node.borrow();
            if borrow.data.split_whitespace().next() == Some(key.as_str()) {
                return Some(borrow.data.clone());
            }
            current = borrow.next.clone();
        }
        None
    }
}
