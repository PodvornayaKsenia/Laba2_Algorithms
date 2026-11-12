use crate::array::Array;
use crate::myset::MySet;
use std::fs;
use std::io;

pub fn execute_set_command(filename: &str, query: &str) -> Result<(), String> {
    let mut parts = query.split_whitespace();
    let command = parts.next().ok_or("Не указана команда")?;
    let name = parts.next().ok_or("Не указано имя множества")?;
    if !matches!(command, "SETADD" | "SETDEL" | "SET_AT" | "SETPRINT") {
        return Err("Неизвестная команда множества".into());
    }
    let value = if command == "SETPRINT" { None } else {
        Some(parts.next().ok_or("Не указан элемент")?)
    };
    if parts.next().is_some() { return Err("Лишние аргументы".into()); }
    let content = match fs::read_to_string(filename) {
        Ok(v) => v,
        Err(e) if e.kind() == io::ErrorKind::NotFound => String::new(),
        Err(e) => return Err(format!("Ошибка чтения файла: {e}")),
    };
    let mut lines = Array::new();
    let mut set = MySet::new();
    let mut position = None;
    for line in content.lines() {
        let mut words = line.split_whitespace();
        if words.next() == Some("SET") && words.next() == Some(name) {
            if position.is_some() { return Err("Множество записано дважды".into()); }
            position = Some(lines.len());
            for word in words { set.insert(word); }
        }
        lines.push(line.to_string());
    }
    let changed = match command {
        "SETADD" => {
            let changed = set.insert(value.unwrap_or(""));
            println!("{}", if changed { "OK" } else { "ALREADY EXISTS" });
            changed
        }
        "SETDEL" => {
            let changed = set.remove(value.unwrap_or(""));
            println!("{}", if changed { "OK" } else { "NOT FOUND" });
            changed
        }
        "SET_AT" => { println!("{}", if set.contains(value.unwrap_or("")) { "TRUE" } else { "FALSE" }); false }
        _ => { println!("{}", set.display()); false }
    };
    if changed {
        let mut line = format!("SET {name}");
        set.for_each(|item| { line.push(' '); line.push_str(item); });
        if let Some(i) = position { lines.replace(i, line); } else { lines.push(line); }
        let mut text = String::new();
        for item in lines.iter() { text.push_str(item); text.push('\n'); }
        fs::write(filename, text).map_err(|e| format!("Ошибка записи файла: {e}"))?;
    }
    Ok(())
}
