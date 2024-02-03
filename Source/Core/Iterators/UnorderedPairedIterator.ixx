// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt_private.UnorderedPairedIterator;

import jpt.TypeDefs;

import jpt_private.LinearNodeIterator;
import jpt_private.ContiguousIterator;

export namespace jpt_private
{
	/** Unordered Map iterator */
	template<typename TKey, typename TData>
	class UnorderedPairedIterator
	{
	private:


	public:

	};

	template<typename TKey, typename TData>
	class ConstUnorderedPairedIterator
	{
	private:


	public:

	};
}	










/*export class UnorderedPairedIterator<T1, T2> implements TypeDefs.PairIterator<T1, T2>
	{
		private _first: LinearNodeIterator<T1>;
		private _second: LinearNodeIterator<T2>;

		constructor(first: LinearNodeIterator<T1>, second: LinearNodeIterator<T2>)
		{
			this._first = first;
			this._second = second;
		}

		public get first(): T1
		{
			return this._first.current;
		}

		public get second(): T2
		{
			return this._second.current;
		}

		public get valid(): boolean
		{
			return this._first.valid && this._second.valid;
		}

		public moveNext(): void
		{
			this._first.moveNext();
			this._second.moveNext();
		}

		public movePrevious(): void
		{
			this._first.movePrevious();
			this._second.movePrevious();
		}

		public moveFirst(): void
		{
			this._first.moveFirst();
			this._second.moveFirst();
		}

		public moveLast(): void
		{
			this._first.moveLast();
			this._second.moveLast();
		}

		public move(index: number): void
		{
			this._first.move(index);
			this._second.move(index);
		}

		public clone(): TypeDefs.PairIterator<T1, T2>
		{
			return new UnorderedPairedIterator(this._first.clone(), this._second.clone());
		}
	}*/
