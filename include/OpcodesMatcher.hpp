#ifndef OPCODESMATCHER_HPP
#define OPCODESMATCHER_HPP

template<typename T>
class OpcodesMatcher
{
    public:
        static unsigned int getActionId(unsigned short opcode)
        {
            for (int i = 0; i < T::NBOPCODE; i++)
            {
                if ((T::mask[i]&opcode) == T::id[i])
                    return i;
            }

            return -1;
        };
    private:
        OpcodesMatcher() = default;
        OpcodesMatcher(const OpcodesMatcher&) = default;
        virtual ~OpcodesMatcher() = default;
};

#endif // OPCODESMATCHER_HPP
