module google.protobuf.common;

alias bytes = ubyte[];

auto defaultValue(T)()
{
    import std.traits : isFloatingPoint;

    static if (isFloatingPoint!T)
        return cast(T) 0.0;
    else
        return T.init;
}

class ProtobufException : Exception
{
    this(string message = null, string file = __FILE__, size_t line = __LINE__,
        Throwable next = null) @safe pure nothrow
    {
        super(message, file, line, next);
    }
}

enum Wire: ubyte
{
    none,
    fixed = 1 << 0,
    zigzag = 1 << 1,
    fixedKey = 1 << 2,
    zigzagKey = 1 << 3,
    fixedValue = fixed,
    zigzagValue = zigzag,
    fixedKeyFixedValue = fixedKey | fixedValue,
    fixedKeyZigzagValue = fixedKey | zigzagValue,
    zigzagKeyFixedValue = zigzagKey | fixedValue,
    zigzagKeyZigzagValue = zigzagKey | zigzagValue,
}

package Wire keyWireToWire(Wire wire)
{
    return cast(Wire)((wire >> 2) & 0x03);
}

package Wire valueWireToWire(Wire wire)
{
    return cast(Wire)(wire & 0x03);
}

struct Proto
{
    import std.typecons : Flag;

    uint tag;
    Wire wire;
    Flag!"packed" packed;
}

template protoByField(alias field)
{
    import std.traits : getUDAs;

    enum Proto protoByField = getUDAs!(field, Proto)[0];
}

enum MapFieldTag: uint
{
    key = 1,
    value = 2,
}

struct Oneof
{
    string caseFieldName;
}

template isOneof(alias field)
{
    import std.traits : hasUDA;

    enum bool isOneof = hasUDA!(field, Oneof);
}

template oneofCaseFieldName(alias field)
{
    import std.traits : getUDAs;

    static assert(isOneof!field, "No oneof field");

    enum string oneofCaseFieldName = getUDAs!(field, Oneof)[0].caseFieldName;
}

template oneofAccessorName(alias field)
{
    static string accessorName()
    {
        import std.algorithm : skipOver;

        static assert(__traits(identifier, field)[0] == '_', "Invalid oneof union member name");

        string fieldName = __traits(identifier, field);
        bool result = fieldName.skipOver('_');
        assert(result, "Oneof field should start with '_'");

        return fieldName;
    }

    enum string oneofAccessorName = accessorName;
}

template oneofAccessors(alias field)
{
    static string generateAccessors()
    {
        import std.string : format;

        enum accessorName = oneofAccessorName!field;
        
        return "
            %1$s %2$s() { return %3$s == typeof(%3$s).%2$s ? _%2$s : (%1$s).init; }
            void %2$s(%1$s _) { _%2$s = _; %3$s = typeof(%3$s).%2$s; }
            ".format(typeof(field).stringof, accessorName, oneofCaseFieldName!field);
    }

    enum oneofAccessors = generateAccessors;
}

template Message(T)
{
    import std.meta : allSatisfy, staticMap, staticSort;
    import std.traits : getSymbolsByUDA;

    static assert(fields.length > 0, "Definition of '" ~ T.stringof ~ "' has no Proto field");
    static assert(allSatisfy!(validateField, fields), "'" ~ T.stringof ~ "' has invalid fields");

    alias fields = staticSort!(Less, unsortedFields);
    alias protos = staticMap!(protoByField, fields);

    alias fieldNames = staticMap!(fieldName, fields);

    private alias unsortedFields = getSymbolsByUDA!(T, Proto);
    private static enum fieldName(alias field) = __traits(identifier, field);
    private static enum Less(alias field1, alias field2) = protoByField!field1.tag < protoByField!field2.tag;
}

unittest
{
    import std.meta : AliasSeq;
    import std.typecons : No;

    static class Test
    {
        @Proto(3) int foo;
        @Proto(2, Wire.fixed) int bar;
    }

    static assert(Message!Test.fieldNames == AliasSeq!("bar", "foo"));
    static assert(Message!Test.protos == AliasSeq!(Proto(2, Wire.fixed, No.packed), Proto(3, Wire.none, No.packed)));
}

template validateField(alias field)
{
    import std.traits : getUDAs;

    enum validateField = validateProto!(getUDAs!(field, Proto)[0], typeof(field));
}

bool validateProto(Proto proto, T)()
{
    import std.range : ElementType;
    import std.traits : isAggregateType, isArray, isAssociativeArray, isBoolean, isFloatingPoint, isIntegral;
    import std.traits : KeyType, ValueType;

    static assert(proto.tag > 0 && proto.tag < (2 << 29), "Tag value out of range [1 536_870_912]");

    static if (isBoolean!T)
    {
        static assert(proto.wire == Wire.none);
        static assert(!proto.packed);
    }
    else static if (is(T == int) || is(T == uint) || is(T == long) || is(T == ulong))
    {
        import std.algorithm : canFind;

        static assert([Wire.none, Wire.fixed, Wire.zigzag].canFind(proto.wire));
        static assert(!proto.packed);
    }
    else static if (is(T == enum) && is(T : int))
    {
        static assert(proto.wire == Wire.none);
        static assert(!proto.packed);
    }
    else static if (isFloatingPoint!T)
    {
        static assert(proto.wire == Wire.none);
        static assert(!proto.packed);
    }
    else static if (is(T == string) || is(T == bytes))
    {
        static assert(proto.wire == Wire.none);
        static assert(!proto.packed);
    }
    else static if (isArray!T)
    {
        static assert(is(ElementType!T == string) || is(ElementType!T == bytes)
            || (!isArray!(ElementType!T) && !isAssociativeArray!(ElementType!T)));
        enum elementProto = Proto(proto.tag, proto.wire);

        static assert(validateProto!(elementProto, ElementType!T));
    }
    else static if (isAssociativeArray!T)
    {
        static assert(isBoolean!(KeyType!T) || isIntegral!(KeyType!T) || is(KeyType!T == string));
        static assert(is(ValueType!T == string) || is(ValueType!T == bytes)
            || (!isArray!(ValueType!T) && !isAssociativeArray!(ValueType!T)));

        enum keyProto = Proto(MapFieldTag.key, keyWireToWire(proto.wire));
        static assert(validateProto!(keyProto, KeyType!T));

        enum valueProto = Proto(MapFieldTag.value, valueWireToWire(proto.wire));
        static assert(validateProto!(valueProto, ValueType!T));

        static assert(!proto.packed);
    }
    else static if (isAggregateType!T)
    {
        static assert(proto.wire == Wire.none);
        static assert(!proto.packed);
    }
    else
    {
        static assert(0, "Invalid Proto definition for type " ~ T.stringof);
    }

    return true;
}
