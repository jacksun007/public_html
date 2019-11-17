fn hand_score(hand: (char, char)) -> u32 {
    match hand {
        ('A', 'A') => 12,
        ('A', i) | (i, 'A') if i.is_digit(10) =>
            11 + i.to_digit(10).unwrap(),
        ('A', _) | (_, 'A') => 21,
        (i, j) if i.is_digit(10) && j.is_digit(10) =>
            i.to_digit(10).unwrap() + j.to_digit(10).unwrap(),
        (i, _) | (_, i) if i.is_digit(10) =>
            10 + i.to_digit(10).unwrap(),
        _ => 20,
    }
}

fn print_hand_and_score(first: char, second: char) 
{
    println!("{}{}: {}", first, second, hand_score((first, second)));
}

fn main() {
    print_hand_and_score('A', 'K');
    print_hand_and_score('A', 'A');
    print_hand_and_score('T', 'A');
    print_hand_and_score('3', '9');
    print_hand_and_score('K', '5');
    print_hand_and_score('7', 'Q');
    print_hand_and_score('J', 'T');
    print_hand_and_score('9', '9');
    print_hand_and_score('2', '6');
}
