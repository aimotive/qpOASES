/*
 *	This file is part of qpOASES.
 *
 *	qpOASES -- An Implementation of the Online Active Set Strategy.
 *
 *	Copyright (C) 2012 by Andreas Waechter. All rights reserved.
 *
 *	qpOASES is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation; either
 *	version 2.1 of the License, or (at your option) any later version.
 *
 *	qpOASES is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *	See the GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with qpOASES; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


/**
 *	\file include/qpOASES/SparseSolver.hpp
 *	\author Andreas Waechter
 *	\version 3.0
 *	\date 2012
 *
 *	Interfaces to sparse linear solvers that are used in a Schur-complement
 *	implementation in qpOASES.
 */

#ifndef QPOASES_SPARSESOLVER_HPP
#define QPOASES_SPARSESOLVER_HPP

#include <qpOASES/Utils.hpp>

/* typedef for Fortran INTEGER type.  Might be platform dependent! */
typedef int fint;

BEGIN_NAMESPACE_QPOASES


/**
 *	\brief Base class for linear solvers that are used in a Schur-complement
 *	implementation in qpOASES.
 *
 *	\author Andreas Waechter
 *	\version 3.0
 *	\date 2012
 */
class SparseSolver
{
	/*
	 *	PUBLIC MEMBER FUNCTIONS
	 */
	public:
		/** Default constructor. */
		SparseSolver( );

		/** Copy constructor (deep copy). */
		SparseSolver(	const SparseSolver& rhs		/**< Rhs object. */
					);

		/** Destructor. */
		virtual ~SparseSolver( );

		/** Assignment operator (deep copy). */
		virtual SparseSolver& operator=(	const SparseSolver& rhs	/**< Rhs object. */
								);

		/** Set new matrix data.  The matrix is to be provided
		    in the Harwell-Boeing format.  Only the lower
		    triangular part should be set. */
		virtual returnValue setMatrixData( int dim, /**< Dimension of the linear system. */
						   int numNonzeros, /**< Number of nonzeros in the matrix. */
						   const int* const airn, /**< Row indices for each matrix entry. */
						   const int* const acjn, /**< Column indices for each matrix entry. */
						   const real_t* const avals /**< Values for each matrix entry. */
						   ) = 0;

		/** Compute factorization of current matrix.  This method must be called before solve.*/
		virtual returnValue factorize( ) = 0;

		/** Solve linear system with most recently set matrix data. */
		virtual returnValue solve( int dim, /**< Dimension of the linear system. */
					   const real_t* const rhs, /**< Values for the right hand side. */
					   real_t* const sol /**< Solution of the linear system. */
					   ) = 0;

		/** Clears all data structures. */
		virtual returnValue reset( );

		/** Return the number of negative eigenvalues. */
		virtual int getNegativeEigenvalues( );

		/** Return the rank after a factorization */
		virtual int getRank( );

		/** Returns the zero pivots in case the matrix is rank deficient */
		virtual returnValue getZeroPivots( int *&zeroPivots );

	/*
	 *	PROTECTED MEMBER FUNCTIONS
	 */
	protected:
		/** Frees all allocated memory.
		 *  \return SUCCESSFUL_RETURN */
		returnValue clear( );

		/** Copies all members from given rhs object.
		 *  \return SUCCESSFUL_RETURN */
		returnValue copy(	const SparseSolver& rhs	/**< Rhs object. */
							);

	/*
	 *	PROTECTED MEMBER VARIABLES
	 */
	protected:
};


#ifdef SOLVER_MA27

/**
 *	\brief Implementation of the linear solver interface using Harwell's MA27.
 *
 *	\author Andreas Waechter
 *	\version 3.0
 *	\date 2012
 */
class Ma27SparseSolver: public SparseSolver
{
	/*
	 *	PUBLIC MEMBER FUNCTIONS
	 */
	public:
		/** Default constructor. */
		Ma27SparseSolver( );

		/** Copy constructor (deep copy). */
		Ma27SparseSolver(	const Ma27SparseSolver& rhs		/**< Rhs object. */
					);

		/** Destructor. */
		virtual ~Ma27SparseSolver( );

		/** Assignment operator (deep copy). */
		virtual Ma27SparseSolver& operator=(	const SparseSolver& rhs	/**< Rhs object. */
								);

		/** Set new matrix data.  The matrix is to be provided
		    in the Harwell-Boeing format.  Only the lower
		    triangular part should be set. */
		virtual returnValue setMatrixData( int dim, /**< Dimension of the linear system. */
						   int numNonzeros, /**< Number of nonzeros in the matrix. */
						   const int* const airn, /**< Row indices for each matrix entry. */
						   const int* const acjn, /**< Column indices for each matrix entry. */
						   const real_t* const avals /**< Values for each matrix entry. */
						   );

		/** Compute factorization of current matrix.  This method must be called before solve.*/
		virtual returnValue factorize( );

		/** Solve linear system with most recently set matrix data. */
		virtual returnValue solve( int dim, /**< Dimension of the linear system. */
					   const real_t* const rhs, /**< Values for the right hand side. */
					   real_t* const sol /**< Solution of the linear system. */
					   );

		/** Clears all data structures. */
		virtual returnValue reset( );

		/** Return the number of negative eigenvalues. */
		virtual int getNegativeEigenvalues( );

		/** Return the rank after a factorization */
		virtual int getRank( );

	/*
	 *	PROTECTED MEMBER FUNCTIONS
	 */
	protected:
		/** Frees all allocated memory.
		 *  \return SUCCESSFUL_RETURN */
		returnValue clear( );

		/** Copies all members from given rhs object.
		 *  \return SUCCESSFUL_RETURN */
		returnValue copy(	const Ma27SparseSolver& rhs	/**< Rhs object. */
							);

	/*
	 *	PRIVATE MEMBER FUNCTIONS
	 */
	private:
	/*
	 *	PRIVATE MEMBER VARIABLES
	 */
	private:
		fint dim; /**< Dimension of the current linear system. */

		fint numNonzeros; /**< Number of nonzeros in the current linear system. */

		fint la_ma27; /**< size of a_ma27 (LA in MA27) */

		double* a_ma27; /**< matrix/factor for MA27 (A in MA27).  If have_factorization is false, it contains the matrix entries (and has length numNonzeros), otherwise the factor (and has length la_ma27). */

		fint* irn_ma27; /**< Row entries of matrix (IRN in MA27) */

		fint* jcn_ma27; /**< Column entries of matrix (JCN in MA27) */

		fint icntl_ma27[30];  /**< integer control values (ICNRL in MA27) */

		double cntl_ma27[5];  /**< real control values (CNRL in MA27) */

		fint liw_ma27;  /**< length of integer work space (LIW in MA27) */

		fint* iw_ma27; /**< integer work space (IW in MA27) */

		fint* ikeep_ma27;  /**< IKEEP in MA27 */

		fint nsteps_ma27;  /**< NSTEPS in MA27 */

		fint maxfrt_ma27;  /**< MAXFRT in MA27 */

		bool have_factorization; /**< flag indicating whether factorization for current matrix has already been computed */

		fint neig;	/**< number of negative eigenvalues */

		fint rank;	/**< rank of matrix */
};

#endif // SOLVER_MA27


#ifdef SOLVER_MA57

/**
 *	\brief Implementation of the linear solver interface using Harwell's MA57.
 *
 *	\author Dennis Janka
 *	\version 3.0
 *	\date 2012
 */
class Ma57SparseSolver: public SparseSolver
{
	/*
	 *	PUBLIC MEMBER FUNCTIONS
	 */
	public:
		/** Default constructor. */
		Ma57SparseSolver( );

		/** Copy constructor (deep copy). */
		Ma57SparseSolver(	const Ma57SparseSolver& rhs		/**< Rhs object. */
					);

		/** Destructor. */
		virtual ~Ma57SparseSolver( );

		/** Assignment operator (deep copy). */
		virtual Ma57SparseSolver& operator=(	const SparseSolver& rhs	/**< Rhs object. */
								);

		/** Set new matrix data.  The matrix is to be provided
		    in the Harwell-Boeing format.  Only the lower
		    triangular part should be set. */
		virtual returnValue setMatrixData( int dim, /**< Dimension of the linear system. */
						   int numNonzeros, /**< Number of nonzeros in the matrix. */
						   const int* const airn, /**< Row indices for each matrix entry. */
						   const int* const acjn, /**< Column indices for each matrix entry. */
						   const real_t* const avals /**< Values for each matrix entry. */
						   );

		/** Compute factorization of current matrix.  This method must be called before solve.*/
		virtual returnValue factorize( );

		/** Solve linear system with most recently set matrix data. */
		virtual returnValue solve( int dim, /**< Dimension of the linear system. */
					   const real_t* const rhs, /**< Values for the right hand side. */
					   real_t* const sol /**< Solution of the linear system. */
					   );

		/** Clears all data structures. */
		virtual returnValue reset( );

		/** Return the number of negative eigenvalues. */
		virtual int getNegativeEigenvalues( );

		/** Return the rank after a factorization */
		virtual int getRank( );

		/** Returns the zero pivots in case the matrix is rank deficient */
		virtual returnValue getZeroPivots( int *&zeroPivots );
	/*
	 *	PROTECTED MEMBER FUNCTIONS
	 */
	protected:
		/** Frees all allocated memory.
		 *  \return SUCCESSFUL_RETURN */
		returnValue clear( );

		/** Copies all members from given rhs object.
		 *  \return SUCCESSFUL_RETURN */
		returnValue copy(	const Ma57SparseSolver& rhs	/**< Rhs object. */
							);

	/*
	 *	PRIVATE MEMBER FUNCTIONS
	 */
	private:
	/*
	 *	PRIVATE MEMBER VARIABLES
	 */
	private:
		fint dim;				/**< Dimension of the current linear system. */

		fint numNonzeros;		/**< Number of nonzeros in the current linear system. */

		double* a_ma57;			/**< matrix for MA57 (A in MA57) */

		fint* irn_ma57;			/**< Row entries of matrix (IRN in MA57) */

		fint* jcn_ma57;			/**< Column entries of matrix (JCN in MA57) */

		fint icntl_ma57[30];	/**< integer control values (ICNRL in MA57) */

		double cntl_ma57[5];	/**< real control values (CNRL in MA57) */

		double* fact_ma57;		/**< array for storing the factors */

		fint lfact_ma57;		/**< length of fact_ma57 */

		fint* ifact_ma57;		/**< indexing information about the factors */

		fint lifact_ma57;		/**< length of ifact_ma57 */

		bool have_factorization;/**< flag indicating whether factorization for current matrix has already been computed */

		fint neig;				/**< number of negative eigenvalues */

		fint rank;				/**< rank of matrix */

		fint* pivots;			/**< sequence of pivots used in factorization */
};

#endif // SOLVER_MA57


#ifdef SOLVER_NONE

/**
 *	\brief Implementation of a dummy sparse solver. An error is thrown if a factorization is attempted.
 *
 *	\author Dennis Janka
 *	\version 3.1
 *	\date 2015
 */
class DummySparseSolver: public SparseSolver
{
	/*
	 *	PUBLIC MEMBER FUNCTIONS
	 */
	public:
		/** Set new matrix data.  The matrix is to be provided
		    in the Harwell-Boeing format.  Only the lower
		    triangular part should be set. */
		virtual returnValue setMatrixData( int dim, /**< Dimension of the linear system. */
						   int numNonzeros, /**< Number of nonzeros in the matrix. */
						   const int* const airn, /**< Row indices for each matrix entry. */
						   const int* const acjn, /**< Column indices for each matrix entry. */
						   const real_t* const avals /**< Values for each matrix entry. */
						   );

		/** Compute factorization of current matrix.  This method must be called before solve.*/
		virtual returnValue factorize( );

		/** Solve linear system with most recently set matrix data. */
		virtual returnValue solve( int dim, /**< Dimension of the linear system. */
					   const real_t* const rhs, /**< Values for the right hand side. */
					   real_t* const sol /**< Solution of the linear system. */
					   );
};

#endif // SOLVER_NONE


END_NAMESPACE_QPOASES

#endif	/* QPOASES_SPARSESOLVER_HPP */

/*
 *	end of file
 */
