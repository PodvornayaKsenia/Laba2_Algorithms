mod array;
mod double_list;
mod genom;
mod hash_task;
mod huffman_task;
mod lru_task;
mod myset;
mod set_commands;
mod single_list;
mod stack;
mod xml;
mod ydarenia;

use std::env;
use std::io::{self, Write};
use array::Array;
use hash_task::HashTable;
use huffman_task::HuffmanCoder;
use lru_task::LRUCache;

fn read_line() -> Option<String> {
    let mut line = String::new();
    if io::stdin().read_line(&mut line).ok()? == 0 { return None; }
    Some(line.trim_end_matches(|c| c == '\r' || c == '\n').to_string())
}
fn prompt(message: &str) -> Option<String> {
    print!("{message}");
    io::stdout().flush().ok()?;
    read_line()
}
fn run_xml() {
    println!("\nЗадание 1. Восстановление XML");
    if let Some(xml) = prompt("Введите XML-строку: ") {
        println!("Результат: {}", xml::xml_restore(&xml));
    }
}
fn run_set() {
    println!("\nЗадание 2. АТД Множество");
    let filename = match prompt("Имя файла: ") { Some(v) if !v.is_empty() => v, _ => return };
    let query = match prompt("Команда: ") { Some(v) if !v.is_empty() => v, _ => return };
    if let Err(e) = set_commands::execute_set_command(&filename, &query) { eprintln!("{e}"); }
}
fn run_genome() {
    println!("\nЗадание 3. Инопланетный геном");
    let first = match prompt("Первый геном: ") { Some(v) => v, None => return };
    let second = match prompt("Второй геном: ") { Some(v) => v, None => return };
    println!("Степень близости: {}", genom::genome_similarity(&first, &second));
}
fn run_stresses() {
    println!("\nЗадание 4. Контрольная по ударениям");
    let n = match prompt("Количество слов в словаре: ").and_then(|v| v.trim().parse::<usize>().ok()) {
        Some(v) => v,
        None => { println!("Некорректное количество слов."); return; }
    };
    println!("Введите слова словаря:");
    let mut dictionary = Array::new();
    while dictionary.len() < n {
        let line = match read_line() { Some(v) => v, None => return };
        for word in line.split_whitespace() {
            if dictionary.len() == n { break; }
            dictionary.push(word.to_string());
        }
    }
    let text = match prompt("Текст для проверки: ") { Some(v) => v, None => return };
    println!("Количество ошибок: {}", ydarenia::check_stresses(&dictionary, &text));
}
fn run_huffman() {
    println!("\nЗадание 5. Алгоритм Хаффмана");
    let text = match prompt("Строка для кодирования: ") { Some(v) => v, None => return };
    let mut coder = HuffmanCoder::new();
    coder.build_tree(&text);
    print!("Таблица кодов:\n{}", coder.codes_text());
    let encoded = match coder.encode(&text) { Ok(v) => v, Err(e) => { println!("Ошибка: {e}"); return; } };
    println!("Код: {encoded}");
    match coder.decode(&encoded) {
        Ok(v) => println!("Декодировано: {v}"),
        Err(e) => println!("Ошибка: {e}"),
    }
    let bits = match prompt("Введите двоичный код (Enter — оставить полученный): ") { Some(v) => v, None => return };
    let bits = if bits.is_empty() { encoded } else { bits };
    match coder.decode(&bits) {
        Ok(v) => println!("Результат: {v}"),
        Err(e) => println!("Ошибка: {e}"),
    }
}
fn hash_comparison() {
    use std::time::Instant;
    const CAPACITY: usize = 23;
    const REPEATS: usize = 3;
    let amounts = [6usize, 11, 17, 21];
    println!("\nЭмпирическое сравнение: таблица на {CAPACITY} ячейки, {REPEATS} повторения");
    for amount in amounts {
        println!("\n{} пар, заполнение {:.1}%", amount, amount as f64 * 100.0 / CAPACITY as f64);
        for method in 0..2 {
            let mut insert_time = 0.0;
            let mut found_time = 0.0;
            let mut missing_time = 0.0;
            let mut insert_probes = 0.0;
            let mut found_probes = 0.0;
            let mut missing_probes = 0.0;
            for _ in 0..REPEATS {
                let mut table = HashTable::new(method == 1, CAPACITY);
                let start = Instant::now();
                for i in 0..amount {
                    let key = format!("key{i}");
                    let value = format!("value{i}");
                    table.insert(&key, &value);
                }
                insert_time += start.elapsed().as_secs_f64() * 1_000_000.0;
                insert_probes += table.probes() as f64 / amount as f64;
                table.reset_probes();
                let start = Instant::now();
                for i in 0..amount {
                    let key = format!("key{i}");
                    assert!(table.get(&key).is_some());
                }
                found_time += start.elapsed().as_secs_f64() * 1_000_000.0;
                found_probes += table.probes() as f64 / amount as f64;
                table.reset_probes();
                let start = Instant::now();
                for i in 0..amount {
                    let key = format!("missing{i}");
                    assert!(!table.contains_without_bloom(&key));
                }
                missing_time += start.elapsed().as_secs_f64() * 1_000_000.0;
                missing_probes += table.probes() as f64 / amount as f64;
            }
            let name = if method == 0 { "Линейное пробирование" } else { "Двойное хеширование" };
            println!("{name}:");
            println!("  Время серии, мкс: вставка {:.6}, успешный поиск {:.6}, безуспешный поиск {:.6}",
                insert_time / REPEATS as f64, found_time / REPEATS as f64, missing_time / REPEATS as f64);
            println!("  Проверок на операцию: вставка {:.3}, успешный поиск {:.3}, безуспешный поиск {:.3}",
                insert_probes / REPEATS as f64, found_probes / REPEATS as f64, missing_probes / REPEATS as f64);
        }
    }
}
fn run_hash() {
    let mut linear = HashTable::new(false, 1009);
    let mut double = HashTable::new(true, 1009);
    loop {
        println!("\nЗадание 6. Хеш-таблица");
        println!("1. Добавить пару ключ-значение\n2. Найти значение по ключу\n3. Удалить ключ\n4. Вывести таблицы\n5. Эмпирическое сравнение\n0. Назад");
        let choice = match prompt("Выбор: ").and_then(|v| v.trim().parse::<u32>().ok()) { Some(v) => v, None => return };
        match choice {
            0 => return,
            1..=3 => {
                let key = match prompt("Введите ключ: ") { Some(v) => v, None => return };
                if choice == 1 {
                    let value = match prompt("Введите значение: ") { Some(v) => v, None => return };
                    let a = linear.insert(&key, &value);
                    let b = double.insert(&key, &value);
                    if a && b { println!("Пара добавлена."); }
                    else if linear.contains(&key) && double.contains(&key) { println!("Значение обновлено."); }
                    else { println!("Таблица заполнена."); }
                } else if choice == 2 {
                    println!("Фильтр Блума: {}", if linear.bloom_check(&key) { "Возможно присутствует" } else { "Точно отсутствует" });
                    println!("Линейное пробирование: {}", linear.get(&key).unwrap_or("Не найден"));
                    println!("Двойное хеширование: {}", double.get(&key).unwrap_or("Не найден"));
                } else {
                    let a = linear.remove(&key);
                    let b = double.remove(&key);
                    println!("{}", if a && b { "Ключ удален." } else { "Ключ не найден." });
                }
            }
            4 => {
                println!("Линейное пробирование:"); linear.print();
                println!("Двойное хеширование:"); double.print();
            }
            5 => hash_comparison(),
            _ => println!("Нет такого пункта."),
        }
    }
}
fn run_lru() {
    println!("\nЗадание 7. LRU-кэш");
    let first = match prompt("Ёмкость кэша и число запросов: ") { Some(v) => v, None => return };
    let mut tokens = first.split_whitespace();
    let capacity = match tokens.next().and_then(|x| x.parse::<usize>().ok()) { Some(v) => v, None => return };
    let count = match tokens.next().and_then(|x| x.parse::<usize>().ok()) { Some(v) => v, None => return };
    let mut cache = LRUCache::new(capacity);
    println!("Введите команды SET x y или GET x:");
    let mut outputs = String::new();
    for _ in 0..count {
        let line = match read_line() { Some(v) => v, None => return };
        let mut parts = line.split_whitespace();
        match parts.next() {
            Some("SET") => {
                let key = match parts.next().and_then(|s| s.parse::<i32>().ok()) { Some(v) => v, None => return };
                let value = match parts.next().and_then(|s| s.parse::<i32>().ok()) { Some(v) => v, None => return };
                cache.set(key, value);
            }
            Some("GET") => {
                let key = match parts.next().and_then(|s| s.parse::<i32>().ok()) { Some(v) => v, None => return };
                if !outputs.is_empty() { outputs.push(' '); }
                outputs.push_str(&cache.get(key).to_string());
            }
            _ => { println!("Неизвестная команда."); return; }
        }
    }
    if !outputs.is_empty() { println!("{outputs}"); }
}
fn show_menu() {
    println!("\nЛабораторная работа №2");
    println!("1. Восстановление XML");
    println!("2. АТД Множество");
    println!("3. Инопланетный геном");
    println!("4. Контрольная по ударениям");
    println!("5. Алгоритм Хаффмана");
    println!("6. Хеш-таблица и фильтр Блума");
    println!("7. LRU-кэш");
    println!("0. Выход");
}
fn main() {
    let mut args = env::args().skip(1);
    let first_arg = args.next();
    if let Some(first_arg) = first_arg {
        let mut filename: Option<String> = None;
        let mut query: Option<String> = None;
        let mut current = Some(first_arg);
        while let Some(flag) = current {
            match flag.as_str() {
                "--file" => filename = args.next(),
                "--query" => query = args.next(),
                _ => { eprintln!("Формат: ./dbms --file file.data --query 'SETADD myset 10'"); std::process::exit(1); }
            }
            current = args.next();
        }
        match (filename, query) {
            (Some(f), Some(q)) => {
                if let Err(e) = set_commands::execute_set_command(&f, &q) { eprintln!("{e}"); std::process::exit(1); }
            }
            _ => { eprintln!("Необходимо указать --file и --query."); std::process::exit(1); }
        }
        return;
    }
    loop {
        show_menu();
        let choice = match prompt("Выберите задание: ") {
            Some(v) => v.trim().parse::<u32>().ok(),
            None => return,
        };
        match choice {
            Some(1) => run_xml(),
            Some(2) => run_set(),
            Some(3) => run_genome(),
            Some(4) => run_stresses(),
            Some(5) => run_huffman(),
            Some(6) => run_hash(),
            Some(7) => run_lru(),
            Some(0) => return,
            _ => println!("Введите число от 0 до 7."),
        }
    }
}
