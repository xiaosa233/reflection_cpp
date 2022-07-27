import click

'''
    Generating the c++ macro like :
    #define _f_select(_16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, n, ...) n
    #define _n 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
    #define _f{}(func, limiter, class, args, ...) _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
    {} from 2 --> N
'''
@click.command()
@click.option('--n', default=64, help='value of the common N')
def main(n):
    if n < 2:
        print('No need for n less then 2')
        return

    # generating _f_select
    select_content = '#define _f_select('
    for i in range(n):
        select_content += '_' + str(i+1) + ', '
    select_content += 'n, ...) n'

    #generating n
    n_content = '#define _n '
    num_vec = [str(n-i) for i in range(n)]
    n_content += ', '.join(num_vec)

    #generating _f{}
    f_content_vec = []
    postfix_template = '(func, limiter, class, args, ...) _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)'
    for i in range(n):
        if i==0:
            continue
        f_content_vec.append('#define _f' + str(i+1) + postfix_template)
    f_content = '\n'.join(f_content_vec)

    print(select_content)
    print(n_content,'\n')
    print(f_content)    

    input('press to close')

if __name__ == '__main__':
    main()
