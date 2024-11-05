#region Copyright notice and license
// Protocol Buffers - Google's data interchange format
// Copyright 2015 Google Inc.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd
#endregion

using System;
using Google.Protobuf.Collections;

namespace Google.Protobuf
{
    /// <summary>
    /// A set of extension methods on <see cref="RepeatedField{T}"/>
    /// </summary>
    public static class RepeatedFieldExtensions
    {
        /// <summary>Adds the elements of the specified span to the end of the <see cref="RepeatedField{T}"/>.</summary>
        /// <typeparam name="T">The type of elements in the <see cref="RepeatedField{T}"/>.</typeparam>
        /// <param name="repeatedField">The list to which the elements should be added.</param>
        /// <param name="source">The span whose elements should be added to the end of the <see cref="RepeatedField{T}"/>.</param>
        /// <exception cref="ArgumentNullException">The <paramref name="repeatedField"/> is null.</exception>
        public static void AddRange<T>(this RepeatedField<T> repeatedField, ReadOnlySpan<T> source)
        {
            ProtoPreconditions.CheckNotNull(repeatedField, nameof(repeatedField));
            repeatedField.AddRangeSpan(source);
        }
    }
}