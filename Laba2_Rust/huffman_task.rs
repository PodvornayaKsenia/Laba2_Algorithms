use crate::array::{Array, DynArray};

struct HuffmanNode {
    symbol: u8,
    frequency: usize,
    left: Option<usize>,
    right: Option<usize>,
}

pub struct HuffmanCoder {
    nodes: DynArray<HuffmanNode>,
    root: Option<usize>,
    codes: Array,
}
impl HuffmanCoder {
    pub fn new() -> Self {
        {
            let mut codes = Array::new();
            for _ in 0..256 { codes.push(String::new()); }
            Self { nodes: DynArray::new(), root: None, codes }
        }
    }
    fn build_codes(&mut self, index: usize, code: String) {
        let left = self.nodes[index].left;
        let right = self.nodes[index].right;
        if left.is_none() && right.is_none() {
            let symbol = self.nodes[index].symbol as usize;
            self.codes.replace(symbol, if code.is_empty() { "0".into() } else { code });
            return;
        }
        if let Some(i) = left { self.build_codes(i, format!("{code}0")); }
        if let Some(i) = right { self.build_codes(i, format!("{code}1")); }
    }
    pub fn build_tree(&mut self, text: &str) {
        self.nodes.clear();
        self.root = None;
        for i in 0..self.codes.len() { self.codes.replace(i, String::new()); }
        let mut frequencies = [0usize; 256];
        for b in text.bytes() { frequencies[b as usize] += 1; }
        let mut free_nodes = Array::new();
        for (symbol, freq) in frequencies.iter().enumerate() {
            if *freq == 0 { continue; }
            let index = self.nodes.len();
            self.nodes.push(HuffmanNode { symbol: symbol as u8, frequency: *freq, left: None, right: None });
            free_nodes.push(index.to_string());
        }
        while free_nodes.len() > 1 {
            let first = self.remove_min(&mut free_nodes);
            let second = self.remove_min(&mut free_nodes);
            let index = self.nodes.len();
            let freq = self.nodes[first].frequency + self.nodes[second].frequency;
            self.nodes.push(HuffmanNode { symbol: 0, frequency: freq, left: Some(first), right: Some(second) });
            free_nodes.push(index.to_string());
        }
        if let Some(last) = free_nodes.get(0) {
            let index = last.parse::<usize>().unwrap_or(0);
            self.root = Some(index);
            self.build_codes(index, String::new());
        }
    }
    fn remove_min(&self, free_nodes: &mut Array) -> usize {
        let mut best_position = 0;
        for i in 1..free_nodes.len() {
            let current = free_nodes.get(i).unwrap_or("0").parse::<usize>().unwrap_or(0);
            let best = free_nodes.get(best_position).unwrap_or("0").parse::<usize>().unwrap_or(0);
            if self.nodes[current].frequency < self.nodes[best].frequency { best_position = i; }
        }
        let index = free_nodes.get(best_position).unwrap_or("0").parse().unwrap_or(0);
        free_nodes.remove(best_position);
        index
    }
    pub fn codes_text(&self) -> String {
        let mut output = String::new();
        for (i, code) in self.codes.iter().enumerate() {
            if code.is_empty() { continue; }
            let symbol = match i {
                32 => "[пробел]".to_string(),
                9 => "[табуляция]".to_string(),
                10 => "[перевод строки]".to_string(),
                _ => (i as u8 as char).to_string(),
            };
            output.push_str(&format!("{symbol}: {code}\n"));
        }
        output
    }
    pub fn encode(&self, text: &str) -> Result<String, String> {
        let mut output = String::new();
        for b in text.bytes() {
            let code = self.codes.get(b as usize).unwrap_or("");
            if code.is_empty() { return Err("Символ отсутствует в таблице кодов".into()); }
            output.push_str(code);
        }
        Ok(output)
    }
    pub fn decode(&self, bits: &str) -> Result<String, String> {
        let root = match self.root {
            Some(i) => i,
            None if bits.is_empty() => return Ok(String::new()),
            None => return Err("Дерево пустое".into()),
        };
        let node = &self.nodes[root];
        if node.left.is_none() && node.right.is_none() {
            if bits.bytes().any(|b| b != b'0') { return Err("Некорректный двоичный код".into()); }
            let mut output = String::new();
            let bytes = std::iter::repeat(node.symbol).take(bits.len()).collect::<Box<[u8]>>();
            output.push_str(std::str::from_utf8(&bytes).map_err(|e| e.to_string())?);
            return Ok(output);
        }
        let mut output = DynArray::<u8>::new();
        let mut current = root;
        for bit in bits.bytes() {
            current = match bit {
                b'0' => self.nodes[current].left,
                b'1' => self.nodes[current].right,
                _ => return Err("Код должен содержать только 0 и 1".into()),
            }.ok_or("Некорректный двоичный код")?;
            if self.nodes[current].left.is_none() && self.nodes[current].right.is_none() {
                output.push(self.nodes[current].symbol);
                current = root;
            }
        }
        if current != root { return Err("Двоичный код обрывается".into()); }
        {
            let mut bytes = std::iter::repeat(0u8).take(output.len()).collect::<Box<[u8]>>();
            for i in 0..output.len() { bytes[i] = output[i]; }
            Ok(std::str::from_utf8(&bytes).map_err(|e| e.to_string())?.to_string())
        }
    }
}
impl Default for HuffmanCoder { fn default() -> Self { Self::new() } }
