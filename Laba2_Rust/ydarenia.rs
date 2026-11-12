use crate::array::Array;

fn to_lower(word: &str) -> String { word.to_ascii_lowercase() }
fn count_stresses(word: &str) -> usize {
    word.bytes().filter(|c| c.is_ascii_uppercase()).count()
}
pub fn check_stresses(dictionary: &Array, text: &str) -> usize {
    let mut errors = 0;
    for word in text.split_whitespace() {
        let lower = to_lower(word);
        let mut found = false;
        let mut correct = false;
        for i in 0..dictionary.len() {
            if let Some(dict) = dictionary.get(i) {
                if to_lower(dict) == lower {
                    found = true;
                    if dict == word { correct = true; break; }
                }
            }
        }
        if (found && !correct) || (!found && count_stresses(word) != 1) { errors += 1; }
    }
    errors
}
