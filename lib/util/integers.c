// efficient shortcut for 32 bit values
int get_num_digits(int i) {
  int n = 1;

  if ( i >= 100000000 ) { n += 8; i /= 100000000; }
  if ( i >= 10000     ) { n += 4; i /= 10000; }
  if ( i >= 100       ) { n += 2; i /= 100; }
  if ( i >= 10        ) { n += 1; }

  return n;
}
