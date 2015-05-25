#include <ruby.h>

#define MIN2(a, b) (((a) < (b)) ? (a) : (b))
#define MIN3(a, b, c) (MIN2(MIN2((a), (b)), (c)))

#define MALLOC_W(ptr, size) do { \
    ptr = malloc(size);          \
    if (!ptr)                    \
        rb_memerror();           \
} while (0)

static long min(long *array, long len)
{
    long i, min = array[0];

    for (i = 1; i < len; i++)
        if (array[i] < min)
            min = array[i];

    return min;
}

VALUE distance_ext(VALUE self, VALUE v_from, VALUE v_to, VALUE v_from_len, VALUE v_to_len,
                   VALUE v_max_distance, VALUE v_insertion_cost, VALUE v_deletion_cost,
                   VALUE v_substitution_cost);

void
Init_iterative_with_two_matrix_rows_ext(void)
{
    VALUE StringMetric;
    VALUE Levenshtein;
    VALUE IterativeWithTwoMatrixRowsExt;

    StringMetric = rb_define_module("StringMetric");
    Levenshtein = rb_define_module_under(StringMetric, "Levenshtein");

    IterativeWithTwoMatrixRowsExt =
        rb_define_class_under(Levenshtein, "IterativeWithTwoMatrixRowsExt", rb_cObject);

    rb_define_singleton_method(IterativeWithTwoMatrixRowsExt, "distance_ext", distance_ext, 8);
}

VALUE
distance_ext(VALUE self, VALUE v_from, VALUE v_to, VALUE v_from_len, VALUE v_to_len,
             VALUE v_max_distance, VALUE v_insertion_cost, VALUE v_deletion_cost,
             VALUE v_substitution_cost)
{
    /* Ruby to C datatype conversions & variables declaration-initialization */
    long max_distance =      NUM2LONG(v_max_distance);
    long insertion_cost =    NUM2LONG(v_insertion_cost);
    long deletion_cost =     NUM2LONG(v_deletion_cost);
    long substitution_cost = NUM2LONG(v_substitution_cost);

    long from_len = NUM2LONG(v_from_len);
    long to_len =   NUM2LONG(v_to_len);

    long v0_len = (from_len + 1) * sizeof(long);

    int *from, *to;
    long *v0, current, i, j, sub_cell, ins_cell, cost, x = 0;

    /* Use malloc for these arrays in order to avoid stack overflow from big input strings */
    MALLOC_W(from, from_len * sizeof(int));
    MALLOC_W(to,   to_len   * sizeof(int));
    MALLOC_W(v0,   v0_len);

    /* Fill ´from´ & ´to´ C arrays with values from the corresponding ruby arrays */
    for (i = 0; i < from_len; i++)
        from[i] = NUM2INT(rb_ary_entry(v_from, i));

    for (i = 0; i < to_len; i++)
        to[i] = NUM2INT(rb_ary_entry(v_to, i));

    /* Beggining of the algorithm */
    for (i = 0; i <= from_len; i++)
        v0[i] = i;

    for (i = 0; i < to_len; i++)
    {
        current = x = i + 1;
        sub_cell = v0[0];

        for (j = 0; j < from_len; j++)
        {
            cost = (from[j] == to[i]) ? 0 : substitution_cost;

            ins_cell = v0[j + 1];

            x = MIN3(current + deletion_cost, ins_cell + insertion_cost, sub_cell + cost);

            v0[j] = current;
            current = x;
            sub_cell = ins_cell;
        }

        v0[from_len] = x;
        if (max_distance && min(v0, v0_len) > max_distance)
            break;
    }

    /* Clean up allocated memory */
    free(from);
    free(to);
    free(v0);

    if (max_distance && x > max_distance)
      return LONG2NUM(max_distance);
    else
      return LONG2NUM(x);
}
