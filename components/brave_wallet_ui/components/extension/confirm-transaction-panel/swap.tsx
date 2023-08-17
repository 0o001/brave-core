// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { skipToken } from '@reduxjs/toolkit/query/react'
import Button from '@brave/leo/react/button'
import Alert from '@brave/leo/react/alert'

// Utils
import { WalletSelectors } from '../../../common/selectors'
import { getLocale } from '../../../../common/locale'
import Amount from '../../../utils/amount'
import { openBlockExplorerURL } from '../../../utils/block-explorer-utils'
import { getCoinFromTxDataUnion } from '../../../utils/network-utils'

// Styled components
import {
  HeaderTitle,
  ExchangeRate,
  SwapDetails,
  SwapDetailsDivider,
  SwapAssetContainer,
  SwapAssetAddress,
  SwapAssetTitle,
  AddressOrb,
  AccountNameText,
  SwapAssetHeader,
  SwapDetailsArrow,
  SwapDetailsArrowContainer,
  SwapAssetDetailsContainer,
  AssetIcon,
  SwapAmountColumn,
  NetworkDescriptionText,
  Spacer,
  SwapAssetAmountSymbol,
} from './swap.style'
import { EditButton, NetworkText, StyledWrapper, TopRow } from './style'
import { CreateNetworkIcon, LoadingSkeleton, withPlaceholderIcon } from '../../shared'
import {
  IconsWrapper as SwapAssetIconWrapper,
  NetworkIconWrapper,
  Row
} from '../../shared/style'
import { NftIcon } from '../../shared/nft-icon/nft-icon'
import { Origin } from './common/origin'
import { EditPendingTransactionGas } from './common/gas'

// Components
import { TransactionQueueSteps } from './common/queue'
import { Footer } from './common/footer'
import AdvancedTransactionSettings from '../advanced-transaction-settings'
import {
  PendingTransactionNetworkFeeAndSettings //
} from '../pending-transaction-network-fee/pending-transaction-network-fee'

// Types
import {
  BraveWallet,
  SerializableTransactionInfo
} from '../../../constants/types'
import { UNKNOWN_TOKEN_COINGECKO_ID } from '../../../common/constants/magics'

// Hooks
import { usePendingTransactions } from '../../../common/hooks/use-pending-transaction'
import {
  useGetIsTxSimulationOptInStatusQuery,
  useGetNetworkQuery,
  useLazyGetEVMTransactionSimulationQuery,
  useLazyGetSolanaTransactionSimulationQuery
} from '../../../common/slices/api.slice'
import {
  useUnsafeWalletSelector //
} from '../../../common/hooks/use-safe-selector'

interface Props {
  simulationFailed?: boolean
}

export function ConfirmSwapTransaction ({ simulationFailed }: Props) {
  // redux
  const activeOrigin = useUnsafeWalletSelector(WalletSelectors.activeOrigin)

  // state
  const [showAdvancedTransactionSettings, setShowAdvancedTransactionSettings] =
    React.useState<boolean>(false)
  const [isEditingGas, setIsEditingGas] = React.useState<boolean>(false)

  // hooks
  const {
    transactionDetails,
    fromOrb,
    toOrb,
    updateUnapprovedTransactionNonce,
    selectedPendingTransaction,
    onConfirm,
    onReject,
    queueNextTransaction,
    transactionsQueueLength,
    transactionQueueNumber
  } = usePendingTransactions()

  // queries & mutations
  const { data: makerAssetNetwork } = useGetNetworkQuery(
    transactionDetails?.sellToken ?? skipToken
  )

  const { data: takerAssetNetwork } = useGetNetworkQuery(
    transactionDetails?.buyToken ?? skipToken
  )

  const { data: simulationOptInStatus } = useGetIsTxSimulationOptInStatusQuery()

  const [retryTxSimulationScan] = useLazyGetEVMTransactionSimulationQuery()

  // computed
  const originInfo = selectedPendingTransaction?.originInfo ?? activeOrigin

  // Methods
  const onToggleAdvancedTransactionSettings = () => {
    setShowAdvancedTransactionSettings(!showAdvancedTransactionSettings)
  }
  const onToggleEditGas = () => setIsEditingGas(!isEditingGas)

  // render
  if (
    showAdvancedTransactionSettings &&
    transactionDetails &&
    selectedPendingTransaction
  ) {
    return (
      <AdvancedTransactionSettings
        onCancel={onToggleAdvancedTransactionSettings}
        nonce={transactionDetails.nonce}
        chainId={selectedPendingTransaction.chainId}
        txMetaId={selectedPendingTransaction.id}
        updateUnapprovedTransactionNonce={updateUnapprovedTransactionNonce}
      />
    )
  }

  if (isEditingGas) {
    return <EditPendingTransactionGas onCancel={onToggleEditGas} />
  }

  return (
    <StyledWrapper>
      <TopRow>
        <NetworkText />
        <TransactionQueueSteps
          queueNextTransaction={queueNextTransaction}
          transactionQueueNumber={transactionQueueNumber}
          transactionsQueueLength={transactionsQueueLength}
        />
      </TopRow>

      <HeaderTitle>{getLocale('braveWalletSwapReviewHeader')}</HeaderTitle>

      <Origin originInfo={originInfo} />

      {transactionDetails?.sellToken &&
        transactionDetails?.buyToken &&
        transactionDetails?.minBuyAmount &&
        transactionDetails?.sellAmount &&
        transactionDetails?.buyToken?.coingeckoId !==
          UNKNOWN_TOKEN_COINGECKO_ID &&
        transactionDetails?.sellToken?.coingeckoId !==
          UNKNOWN_TOKEN_COINGECKO_ID && (
          <ExchangeRate>
            1 {transactionDetails.sellToken.symbol} ={' '}
            {transactionDetails.minBuyAmount
              .div(transactionDetails.sellAmount)
              .format(6)}{' '}
            {transactionDetails.buyToken.symbol}
          </ExchangeRate>
        )}
      <SwapDetails>
        <SwapAsset
          type={'maker'}
          network={makerAssetNetwork}
          address={transactionDetails?.senderLabel}
          orb={fromOrb}
          expectAddress={true}
          asset={transactionDetails?.sellToken}
          amount={transactionDetails?.sellAmount}
        />

        <SwapDetailsDivider />
        <SwapDetailsArrowContainer>
          <SwapDetailsArrow />
        </SwapDetailsArrowContainer>

        <SwapAsset
          type={'taker'}
          network={takerAssetNetwork}
          address={transactionDetails?.recipientLabel}
          orb={toOrb}
          expectAddress={false} // set to true once Swap+Send is supported
          asset={transactionDetails?.buyToken}
          amount={transactionDetails?.minBuyAmount}
        />
      </SwapDetails>

      <PendingTransactionNetworkFeeAndSettings
        onToggleAdvancedTransactionSettings={
          onToggleAdvancedTransactionSettings
        }
        onToggleEditGas={onToggleEditGas}
      />

      {simulationFailed &&
      selectedPendingTransaction &&
      simulationOptInStatus === 'allowed' ? (
        <TxSimulationFailedWarning
          retryEvmTxSimulationScan={retryTxSimulationScan}
          transaction={selectedPendingTransaction}
        />
      ) : null}

      <Footer
        onConfirm={onConfirm}
        onReject={onReject}
        rejectButtonType={'cancel'}
      />
    </StyledWrapper>
  )
}

interface SwapAssetProps {
  type: 'maker' | 'taker'
  network?: BraveWallet.NetworkInfo
  amount?: Amount
  asset?: BraveWallet.BlockchainToken
  address?: string
  orb?: string
  expectAddress?: boolean
}

const ICON_CONFIG = {
  size: 'big',
  marginLeft: 0,
  marginRight: 8
} as const

const AssetIconWithPlaceholder = withPlaceholderIcon(AssetIcon, ICON_CONFIG)
const NftIconWithPlaceholder = withPlaceholderIcon(NftIcon, ICON_CONFIG)

function TxSimulationFailedWarning({
  transaction,
  mode,
  retrySvmTxSimulationScan,
  retryEvmTxSimulationScan
}: {
  transaction: SerializableTransactionInfo
} & (
  | {
      retryEvmTxSimulationScan?: ReturnType<
        typeof useLazyGetEVMTransactionSimulationQuery
      >[0]
      retrySvmTxSimulationScan?: undefined
      mode?: undefined
    }
  | {
      retryEvmTxSimulationScan?: undefined
      retrySvmTxSimulationScan?: ReturnType<
        typeof useLazyGetSolanaTransactionSimulationQuery
      >[0]
      mode: Parameters<
        ReturnType<typeof useLazyGetSolanaTransactionSimulationQuery>[0]
      >[0]['mode']
    }
)) {
  // methods
  const retryTxSimulation = React.useCallback(async () => {
    if (mode !== undefined && retrySvmTxSimulationScan) {
      await retrySvmTxSimulationScan({
        chainId: transaction.chainId,
        id: transaction.id,
        mode: mode
      })
    }

    if (retryEvmTxSimulationScan) {
      await retryEvmTxSimulationScan({
        chainId: transaction.chainId,
        coinType: getCoinFromTxDataUnion(transaction.txDataUnion),
        id: transaction.id
      })
    }
  }, [mode, retryEvmTxSimulationScan, retrySvmTxSimulationScan, transaction])

  //  <FullWidth>
  //    <Alert mode='simple' type={'warning'}>
  //      {/* TODO: locale */}
  //      Transaction preview failed.{' '}
  //      <RetryButton onClick={retrySimulation}>Retry</RetryButton>
  //    </Alert>
  //  </FullWidth>

  // <FullWidth>
  //   <Alert mode='simple' type={'warning'}>
  //     <Row gap={'0px'}>
  //       <Column alignItems='flex-start' justifyContent='flex-start'>
  //         <span>
  //         {/* TODO: locale */}
  //           Tx preview failed.{' '}
  //           <RetryButton as='span' onClick={retrySimulation}>
  //             {/* TODO: locale */}
  //             Retry
  //           </RetryButton>
  //         </span>
  //       </Column>
  //     </Row>
  //   </Alert>
  // </FullWidth>

  // render
  return (
    <Row
      alignItems='flex-start'
      justifyContent='flex-start'
      padding={'0px'}
      margin={'10px 0px 0px 0px'}
      width='100%'
    >
      <Alert type='warning' mode='simple'>
        {/* TODO: locale */}
        Transaction check has failed.
        {retrySvmTxSimulationScan || retryEvmTxSimulationScan ? (
          <div slot='actions'>
            <Button kind='plain' onClick={retryTxSimulation}>
              {/* TODO: locale */}
              Retry
            </Button>
          </div>
        ) : null}
      </Alert>
    </Row>
  )
}

function SwapAsset (props: SwapAssetProps) {
  const { type, address, orb, expectAddress, asset, amount, network } = props

  const networkDescription = React.useMemo(() => {
    if (network) {
      return getLocale('braveWalletPortfolioAssetNetworkDescription')
        .replace('$1', '')
        .replace('$2', network.chainName ?? '')
        .trim()
    }

    return ''
  }, [network])

  // computed
  const isUnknownAsset = asset?.coingeckoId === UNKNOWN_TOKEN_COINGECKO_ID

  return (
    <SwapAssetContainer top={type === 'maker'}>
      <SwapAssetHeader>
        <SwapAssetTitle>
          {type === 'maker'
            ? getLocale('braveWalletSwapReviewSpend')
            : getLocale('braveWalletSwapReviewReceive')}
        </SwapAssetTitle>
        {expectAddress && (
          <SwapAssetAddress>
            {address && orb ? (
              <AddressOrb orb={orb} />
            ) : (
              <LoadingSkeleton width={10} height={10} circle={true} />
            )}

            {address ? (
              <AccountNameText>{address}</AccountNameText>
            ) : (
              <LoadingSkeleton width={84} />
            )}
          </SwapAssetAddress>
        )}
      </SwapAssetHeader>

      <SwapAssetDetailsContainer>
        <SwapAssetIconWrapper>
          {!asset || !asset.logo || !network ? (
            <LoadingSkeleton circle={true} width={40} height={40} />
          ) : (
            <>
              {asset?.isErc721 ? (
                <NftIconWithPlaceholder asset={asset} network={network} />
              ) : (
                <AssetIconWithPlaceholder asset={asset} network={network} />
              )}
              {network && asset.contractAddress !== '' && (
                <NetworkIconWrapper>
                  <CreateNetworkIcon network={network} marginRight={0} />
                </NetworkIconWrapper>
              )}
            </>
          )}
        </SwapAssetIconWrapper>
        <SwapAmountColumn>
          {!networkDescription || !asset || !amount ? (
            <>
              <LoadingSkeleton width={200} height={18} />
              <Spacer />
              <LoadingSkeleton width={200} height={18} />
            </>
          ) : isUnknownAsset ? (
            <>
              <SwapAssetAmountSymbol>{asset.symbol}</SwapAssetAmountSymbol>
              <EditButton
                onClick={openBlockExplorerURL({
                  type: 'token',
                  network,
                  value: asset.contractAddress
                })}
              >
                {getLocale('braveWalletTransactionExplorer')}
              </EditButton>
            </>
          ) : (
            <>
              <SwapAssetAmountSymbol>{amount.formatAsAsset(6, asset.symbol)}</SwapAssetAmountSymbol>
              <NetworkDescriptionText>{networkDescription}</NetworkDescriptionText>
            </>
          )}
        </SwapAmountColumn>
      </SwapAssetDetailsContainer>
    </SwapAssetContainer>
  )
}
