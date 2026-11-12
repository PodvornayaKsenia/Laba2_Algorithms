use crate::myset::MySet;
pub fn genome_similarity(first: &str, second: &str) -> usize {
    let mut pairs = MySet::new();
    for pair in second.as_bytes().windows(2) {
        if let Ok(s) = std::str::from_utf8(pair) { pairs.insert(s); }
    }
    let mut count = 0;
    for pair in first.as_bytes().windows(2) {
        if let Ok(s) = std::str::from_utf8(pair) {
            if pairs.contains(s) { count += 1; }
        }
    }
    count
}
