
extern const unsigned int MAX_INDEX;

unsigned int hashPJW(char* name){
    unsigned int val = 0;
    unsigned int i;
    for(; *name; ++name){
        val = (val << 2) + *name;
        i = val & (~MAX_INDEX);
        if(i){
            val = (val ^ (i >> 12)) & MAX_INDEX; 
        }
    }
    return val;
}



