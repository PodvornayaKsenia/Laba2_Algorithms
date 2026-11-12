pub struct SNode { data: String, next: Option<Box<SNode>> }
#[derive(Default)]
pub struct Stack { top: Option<Box<SNode>> }
impl Stack {
    pub fn new() -> Self { Self { top: None } }
    pub fn push(&mut self, value: String) {
        self.top = Some(Box::new(SNode { data: value, next: self.top.take() }));
    }
    pub fn pop(&mut self) -> Option<String> {
        let mut node = self.top.take()?;
        self.top = node.next.take();
        Some(node.data)
    }
    pub fn top(&self) -> Option<&str> { self.top.as_ref().map(|n| n.data.as_str()) }
    pub fn is_empty(&self) -> bool { self.top.is_none() }
}
