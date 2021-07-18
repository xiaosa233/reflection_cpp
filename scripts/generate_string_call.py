import click

'''
    Generating the c++ macro like :
    template <>
    template <class T1, class T2, class R, class... Args>
    R string_callable<{N}>::call_help(const member_function_wrapper<T1, T2, R(Args...)>& func,
                                    const std::vector<std::string>& args) {
    return func(utils::from_string<std::decay_t<typename args_type<0, Args...>::type>>(args[0]),
                utils::from_string<std::decay_t<typename args_type<1, Args...>::type>>(args[1]),
                //.. to N);
    }
'''

TEMPLATE = 'template <>\n\
template <class T1, class T2, class R, class... Args>\n\
R string_callable<{N}>::call_help(const member_function_wrapper<T1, T2, R(Args...)>& func,\n\
                                 const std::vector<std::string>& args) {'

def create_item(n):
    first_part = TEMPLATE.replace('{N}', str(n))
    second_part = [first_part,'  return func(']
    for i in range(n):
        if i+1 != n:
            second_part.append('    utils::from_string<std::decay_t<typename args_type<{0}, Args...>::type>>(args[{0}]),'.format(i))
        else:
            second_part.append('    utils::from_string<std::decay_t<typename args_type<{0}, Args...>::type>>(args[{0}])'.format(i))
    second_part.append('  );}')

    return '\n'.join(second_part)

@click.command()
@click.option('--n', default=64, help='value of the common N')
def main(n):
    if n < 1:
        print('No need for n less then 1')
        return
    result = []
    for i in range(n):
        result.append(create_item(i))
    print('\n'.join(result))

    input('press to close')

if __name__ == '__main__':
    main()
