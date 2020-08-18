fn findall<'a: 'b, 'b>(text: &'a str, word: &'b str) -> Vec<&'a str> {
    let mut ret = vec![];
    let mut s = text;
    
    loop {
        let i = match s.find(word) {
            Some(i) => i,
            None => break,
        };
        
        ret.push(&s[i..i+word.len()]);
        s = &s[i+word.len()..];
    }
    
    ret
}

fn main() {
    let s = "hello hello world hello world";
    let p = "nothing";
    
    let oc = findall(s, p);

    println!("{:?}", oc);
}
