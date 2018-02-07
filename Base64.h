#ifndef BASE64_H
#define BASE64_H

#include <vector> //data structure that is fairly similar to array but is flexible and dynamic like a List in C#
#include <string>

//research on base64 to get proper explanations
//a-z A-Z 0-9 for base64
//algorithim may use + / etc
//differ from standard to standard which is a pain
//we will use + and / for our example
//mandatory padding character which is = (decoder knows the info to decode string)
//all characters can be transferred to base64
//they can be transferred via mail without any loss of data - extremely important for us
//go to wikipedia base64 to understand

namespace Base64
{
    std::string base64_encode(const std::string &);
    std::string base64_encode_bytes(char const* bytes_to_encode, unsigned int in_len);

    //creating a few salts , maybe 3 salts
    //encryption and decryption keys (must memorize or write them down) to scramble the plaintext
    const std::string &SALT1 = "LM:TB:BB:WRU:+fwePO%&^*4$(";
    const std::string &SALT2 = "_:/_77$1857(S%*(&0SeEW";
    const std::string &SALT3 = "line=wowC++=pwned";

    std::string EncryptB64(std::string s)
    {
        //ReplaceStringInPlace(s, "\r\n", "|nlc|");

        s = base64_encode(s);
        s.insert(7, SALT2); //insert into the 7th character
        s = base64_encode(s);
        s += SALT2 + SALT1;
        s = base64_encode(s);
        s.insert(7, "M");
        return s;
    }

    //triple encode with some modifications (no one will know unless they see the formula)
    std::string EncryptB64Depreciated(std::string s)
    {
        //ReplaceStringInPlace(s, "\r\n", "|nlc|");
        s = SALT1 + s + SALT2 + SALT3;
        s = base64_encode(s);
        s.insert(7, SALT3); //insert into the 7th character
        s += SALT1;
        s = base64_encode(s);
        s += SALT2 + SALT3 + SALT1;
        s = base64_encode(s);
        s.insert(1, "L");
        s.insert(7, "M");
        return s;
    }







    const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; //all base64 values

    std::string base64_encode(const std::string &s)
    {
        std::string ret; //output string
        int val = 0;
        int bits = -6; //no. of bits in a subgroup (6)
        const unsigned int b63 = 0x3F; //unsigned = always positive, HEXADECIMAL of 63 (3F = 63) = 11111111

        //for each loop
        for(const auto &c : s) //why auto? if we dunno type of variable, just auto type, C will be assigned from every character from S string
        {
            val = (val << 8) + c; //left binary shifting, bitwise operation, shift left BY 8 places
            //val = val * 2^8 //equivilant to (val << 8)
            bits += 8; //add to number of bits, base 64 works in octects (per character)
            while( bits  >= 0)
            {
                ret.push_back(BASE64_CODES[(val >> bits) & b63]); //it inserts the map number >> means right shifting & sign means binary ADD
                //101
                //011 PERFORM logical & binary operation
                //001 //1+0 = 0, 0+1 = 0, 1+1 = 1
                bits -= 6; //base64 grouping is 6, stops when its below 0
            }
        }
        if(bits >- 6) //we will push again if there are bits left
        {
            ret.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]); //adding padding === if its not etc etc
        }

        while(ret.size() % 4) //if > 0 true, if = 0 false, need to add till size is multiple of 4 (why not 6?)
        {
            ret.push_back('=');
        }

        return ret;
    }

    std::string base64_encode_bytes(char const* bytes_to_encode, unsigned int in_len) {
      std::string ret;
      int i = 0;
      int j = 0;
      unsigned char char_array_3[3];
      unsigned char char_array_4[4];

      while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
          char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
          char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
          char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
          char_array_4[3] = char_array_3[2] & 0x3f;

          for(i = 0; (i <4) ; i++)
            ret += BASE64_CODES[char_array_4[i]];
          i = 0;
        }
      }

      if (i)
      {
        for(j = i; j < 3; j++)
          char_array_3[j] = '\0';

        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
          ret += BASE64_CODES[char_array_4[j]];

        while((i++ < 3))
          ret += '=';

      }

      return ret;

    }

    std::string base64_decode(std::string const& encoded_string) {
      int in_len = encoded_string.size();
      int i = 0;
      int j = 0;
      int in_ = 0;
      unsigned char char_array_4[4], char_array_3[3];
      std::string ret;

      while (in_len-- && ( encoded_string[in_] != '=')) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
          for (i = 0; i <4; i++)
            char_array_4[i] = BASE64_CODES.find(char_array_4[i]);

          char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
          char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
          char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

          for (i = 0; (i < 3); i++)
            ret += char_array_3[i];
          i = 0;
        }
      }

      if (i) {
        for (j = i; j <4; j++)
          char_array_4[j] = 0;

        for (j = 0; j <4; j++)
          char_array_4[j] = BASE64_CODES.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
      }

      return ret;
    }

    std::vector<BYTE> base64_decode_byte(std::string const& encoded_string) {
      int in_len = encoded_string.size();
      int i = 0;
      int j = 0;
      int in_ = 0;
      BYTE char_array_4[4], char_array_3[3];
      std::vector<BYTE> ret;

      while (in_len-- && ( encoded_string[in_] != '=')) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
          for (i = 0; i <4; i++)
            char_array_4[i] = BASE64_CODES.find(char_array_4[i]);

          char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
          char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
          char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

          for (i = 0; (i < 3); i++)
              ret.push_back(char_array_3[i]);
          i = 0;
        }
      }

      if (i) {
        for (j = i; j <4; j++)
          char_array_4[j] = 0;

        for (j = 0; j <4; j++)
          char_array_4[j] = BASE64_CODES.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
      }

      return ret;
    }
}

//Base64::base64_encode //calling the method in Base64 namespace

#endif
