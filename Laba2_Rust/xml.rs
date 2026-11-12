use crate::stack::Stack;

pub fn xml_check(xml: &str) -> bool {
    let bytes = xml.as_bytes();
    let mut stack = Stack::new();
    let mut i = 0;
    while i < bytes.len() {
        if bytes[i] != b'<' { return false; }
        i += 1;
        let closing = i < bytes.len() && bytes[i] == b'/';
        if closing { i += 1; }
        let start = i;
        while i < bytes.len() && bytes[i].is_ascii_lowercase() { i += 1; }
        if i == start || i >= bytes.len() || bytes[i] != b'>' { return false; }
        let name = &xml[start..i];
        i += 1;
        if closing {
            if stack.pop().as_deref() != Some(name) { return false; }
        } else { stack.push(name.to_string()); }
    }
    stack.is_empty()
}

pub fn xml_restore(xml: &str) -> String {
    let mut result: Box<[u8]> = xml.bytes().collect();
    for i in (0..result.len()).rev() {
        let original = result[i];
        for c in b"abcdefghijklmnopqrstuvwxyz<>/" {
            if *c == original { continue; }
            result[i] = *c;
            if let Ok(candidate) = std::str::from_utf8(&result) {
                if xml_check(candidate) { return candidate.to_string(); }
            }
        }
        result[i] = original;
    }
    "Incorrect".to_string()
}
