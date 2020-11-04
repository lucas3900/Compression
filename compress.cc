/******************************************************************************
 *
 * Name:        Lucas Barusek
 *
 * Date:        5/12/19
 *
 * Huffman Compression
 *
 * Description: This program utilizes huffman compression to compress and
 uncompress files. Using huffman compression the compressed files will be a
 smaller size than the original file. This program also utilizes a min heap as
 the priority queue to create the huffman tree
 *
 ******************************************************************************/

#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "pq.h"

struct hnode {
  /* the structure for the hnode */
  int character;
  size_t count;
  hnode *left;
  hnode *right;
  hnode(int character, size_t count,
	      hnode *left=NULL, hnode *right=NULL) :
    character(character), count(count), left(left), right(right) {}
};


int hnode_cmp(hnode * const & a, hnode * const & b){
  /* comparison function that will determine the placement of the hnode
  within the priority queue. Compares the character's counts, and if
   the character's counts is the same then compares their ascii values */
  if (a -> count < b -> count) return -1;
  if (a -> count > b -> count) return 1;
  return (a -> character) - (b -> character);
}


std::string byte_to_bits(int byte){
  /* takes a character byte and coverts it to its 8-bit binary equivalent */
  std::string result = "";
  size_t i;
  int v;
  for (i = 0, v = 128; i < 8; i++, v /=2)
    if (byte >= v) {
      result += '1';
      byte -= v;
    }
    else
      result += '0';
  return result;
}

int bits_to_byte(std::string bitstr){
  /* take an 8-bit binary string and converts it to its corresponding
  character byte */
	int result = 0;
  int v = 0;
	size_t x;
	for (x = 0, v = 128; x<8; x++, v/=2){
		if (bitstr[x] == '1')
			result += v;
	}
	return result;
}


void get_counts(size_t * counts, size_t & size, std::istream & in){
/* goes through every character in the input file and counts how many times
each character appears and stores that data */

// gets every character in the file, and increases that character's
// character count in the array codes
  while (true){
    int v = in.get();
    if (!in) break;
    if (v >= 0){
      counts[v]++;
      size++;
    }
  }
  // POST:  all characters read from in.
  //        counts[k] = the number of occurrences of k in the istream.
  //        size is the number of characters read
}
hnode * make_tree(size_t * counts){
  /* creates the huffman tree that will be used for compression */

  // creates a priority queue to create the huffman tree
  priority_queue<hnode *> queue(hnode_cmp);

  // adds all the characters and it's counts to the priority queue
  for (size_t i = 0; i < 256; i++){
    if (counts[i] != 0)
      queue.add(new hnode(i, counts[i]));
  }

  // Implements tree-building algorithm removing two
  //  trees, combining and reinserting until queue size is 1.
  while(queue.size() != 1){
    hnode * left = queue.front();
    queue.remove();
    hnode * right = queue.front();
    queue.remove();
    queue.add(new hnode(left->character, left -> count + right->count,
                        left, right));
  }


  //Remove and return the resulting tree.
  hnode *result = queue.front();
  queue.remove();
  return result;
}

void make_codes(hnode *tree, std::string *codes, std::string code=""){

  // base case if there is no tree
  if (!tree) return;

  // appends the binary path to the array if you hit a leaf
  if (!(tree -> left or tree -> right)){
    codes[tree -> character] = code;
  }

  // traverses left and right, appending 0 when traversing left
  // and appending 1 when traversing right
  else{
    make_codes(tree->left, codes, code += '0');
    code.pop_back();
    make_codes(tree->right, codes, code += '1');
  }
}

void write_tree(hnode * tree, std::ostream & ostr){
  /* writes the tree structure to the compressed file */

  //outputs I for internal node if root has a child, and traverses
  // right and left in the tree
    if (tree->left) {
      ostr << 'I';
      write_tree(tree->left, ostr);
      write_tree(tree->right, ostr);
    }
    // outputs L and the character if node is a leaf
    else {
      ostr << 'L' << static_cast<char>(tree->character);
    }
}

hnode * read_tree(std::istream & istr){
  /* Recreates the huffman tree from the overhead of the compressed file */

  // gets each character from the tree structure
  int v = istr.get();

  // if you reach a leaf, store the next character in the overhead in an hnode
  if (v == 'L')
    return new hnode(istr.get(), 0, nullptr, nullptr);

  // recursively calls left and right until you reach a leaf, and then
  // creates an hnode with the left and right subtrees
  else{
    hnode * left = read_tree(istr);
    hnode * right = read_tree(istr);
    return new hnode(left -> character, 0, left, right);
  }
}


void write_compress(std::istream & in, std::string * codes,
                    std::ostream & ostr, size_t & file_size){
    /* writes the byte version of the path to the overhead of the
    compressed file */

  std::string buffer;
  std::string bitstr;

  // compiles the binary path into 8-bit strings, and outputs the corresponding
  // byte to the overhead of the compressed file
  for (size_t _ = 0; _ < file_size; _++){
    int ch = in.get();
	  buffer += codes[ch];
    if (buffer.size() >= 8){
      bitstr = buffer.substr(0, 8);
		  buffer = buffer.substr(8);
      ostr << static_cast<char>(bits_to_byte(bitstr));
    }
  }
  // pads the remaining 8-bit string with 0's and outputs the resulting
  // byte to the overhead
  while ((buffer.size() % 8) != 0){
    buffer += '0';
    if (buffer.size() == 8)
      std::cout << static_cast<char>(bits_to_byte(buffer));
  }
}

void compress(char *filename){
  /* from an inputted text file, compresses the file using huffman compresion*/

  std::ifstream in(filename);
  size_t counts[256] = { }; // initializes all to zero.
  size_t file_size;

  // counts how much each character occurs in the file
  get_counts(counts, file_size, in);
  in.close();

  // return if empty file
  if (file_size == 0)
    return;

  // creates the huffman tree
  hnode * tree = make_tree(counts);

  // creates the binary bath for each character and stores it in codes
  std::string codes[256];
  make_codes(tree, codes);

  //ouputs the file size and the tree structure to the overhead
  std::cout << file_size;
  write_tree(tree, std::cout);

  // opens the file and writes the bytes of the character's paths to the
  // overhead
  in.open(filename);
  write_compress(in, codes, std::cout, file_size);
}

void uncompress(){
  /* uncompresses the file to restore it to it's precompressed form */
  if (std::cin.peek() == EOF)
    return;  // handle empty file

  // reads in the file size and recreates the huffman tree
  size_t file_size;
  std::cin >> file_size;
  hnode *tree = read_tree(std::cin);

  // output keeps track of how much we have outputted, begin traverses
  // the tree, and bit will be each 8-bit string path
  size_t output = 0;
  hnode * begin = tree;
  std::string bit;

  while (true){
    // gets each byte from the overhead and converts it to 8-bit binary path
    int byte = std::cin.get();
    bit = byte_to_bits(byte);
    for (size_t i = 0; i < 8; i++){
      // quits while loop when we outputted the correct numbe of characters
      if (output == file_size) return;

      // outputs the character once we reached a leaf in the tree, increments
      // output, resets the tree to the root, and decrements i because
      // we will have skipped over a character
      if (!(tree -> left or tree -> right)){
        std::cout << static_cast<char>(tree -> character);
        tree = begin;
        output++;
        i--;
      }

      // traverses left if current bit is 0, and traverses right otherwise
      else if (bit[i] == '0')  tree = tree -> left;
      else tree = tree -> right;
    }
  }
}


bool ends_with(const std::string & str, const std::string & suffix)
{
  return str.size() >= suffix.size() and
    str.substr(str.size()-suffix.size()) == suffix;
}

bool is_compress(const std::string & command)
{
  return ends_with(command, "compress") and
    not ends_with(command, "uncompress");
}

int main(int argc, char **argv)
{
  assert(argc==argc);
  if (is_compress(argv[0]))
    if (std::string(argv[1])=="-b") {
      compress(argv[2]);
    }
    else
      compress(argv[1]);
  else
    uncompress();
}
