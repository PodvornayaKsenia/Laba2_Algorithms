pub struct Node { pub data: String, pub next: Option<Box<Node>> }
#[derive(Default)]
pub struct SingleList { pub head: Option<Box<Node>> }
impl SingleList {
    pub fn new() -> Self { Self { head: None } }
    pub fn push_front(&mut self, value: String) {
        self.head = Some(Box::new(Node { data: value, next: self.head.take() }));
    }
    pub fn push_back(&mut self, value: String) {
        let mut link = &mut self.head;
        while let Some(node) = link { link = &mut node.next; }
        *link = Some(Box::new(Node { data: value, next: None }));
    }
    pub fn search(&self, value: &str) -> bool {
        let mut current = self.head.as_deref();
        while let Some(node) = current {
            if node.data == value { return true; }
            current = node.next.as_deref();
        }
        false
    }
    pub fn pop_front(&mut self) -> Option<String> {
        let mut node = self.head.take()?;
        self.head = node.next.take();
        Some(node.data)
    }
    pub fn remove_by_value(&mut self, value: &str) -> bool {
        let mut link = &mut self.head;
        while link.as_ref().is_some_and(|node| node.data != value) {
            link = &mut link.as_mut().expect("Узел существует").next;
        }
        if let Some(mut node) = link.take() {
            *link = node.next.take();
            true
        } else { false }
    }
    pub fn for_each(&self, mut f: impl FnMut(&str)) {
        let mut current = self.head.as_deref();
        while let Some(node) = current {
            f(&node.data);
            current = node.next.as_deref();
        }
    }
    pub fn pop_all_into(&mut self, receiver: &mut impl FnMut(String)) {
        while let Some(value) = self.pop_front() { receiver(value); }
    }
}
